#pragma once

#include <GameDefinitions/Base/Base.h>
#include <imgui.h>
#include <imgui_internal.h>

BEGIN_NS(extui)

// BG3 has multiple frames in flight (i.e. pending render) depending on settings;
// IMGUI assumes textures only need to survive a single frame, so its possible for font
// textures to be deleted while a pending BG3 frame  (and IMGUI command buffer) still references them.
// 
// To fix this, we create a copy of font textures and retain them multiple frames to ensure they
// don't go away before rendering is done.
class TextureMirrorSet
{
public:
    // Called by UI thread, after ImGui::Render() and cloning the draw lists
    void Sync(ImDrawData& drawData)
    {
        UpdateMirrors();

        for (auto tex : ImGui::GetPlatformIO().Textures) {
            SyncTexture(tex);
        }

        // Rewrite the command buffer to redirect textures from the IMGUI ones (that are not safe to use) to the SE mirror copies.
        for (auto list : drawData.CmdLists) {
            for (auto& cmd : list->CmdBuffer) {
                if (cmd.TexRef._TexData == nullptr) continue;

                auto mirror = static_cast<ImTextureData*>(cmd.TexRef._TexData->QueueUserData);
                if (mirror != nullptr) {
                    cmd.TexRef._TexData = mirror;
                } else {
                    // Use an empty clip rect to "fix" draws referencing destroyed textures
                    cmd.ClipRect = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
                }
            }
        }

        drawData.Textures = &mirrors_;
    }

    // Render thread, before RenderDrawData()
    void BeginRender()
    {
        for (auto mirror : mirrors_) {
            if (mirror->Status == ImTextureStatus_WantDestroy) {
                // Count number of frames since the delete command was issued to track when it is safe to destroy the mirror texture
                mirror->UnusedFrames++;
            }
        }
    }

    // Releases all mirrors before shutting down the backend; IMGUI textures will be mirrored again after reinitialization
    void Destroy(void (*updateTexture)(ImTextureData*))
    {
        for (auto tex : ImGui::GetPlatformIO().Textures) {
            if (tex->QueueUserData != nullptr) {
                tex->QueueUserData = nullptr;
                // Requests recreation, or completes destruction if the texture was being destroyed anyway
                tex->SetStatus(ImTextureStatus_Destroyed);
            }
        }

        for (auto mirror : mirrors_) {
            if (mirror->Status != ImTextureStatus_Destroyed) {
                RetireMirror(mirror);
                mirror->UnusedFrames = INT_MAX;
                updateTexture(mirror);
            }

            IM_DELETE(mirror);
        }

        mirrors_.clear();
    }

private:
    ImVector<ImTextureData*> mirrors_;

    // Similar to what ImTextureDataUpdateNewFrame() does but without frame counting logic
    void UpdateMirrors()
    {
        for (int i = 0; i < mirrors_.Size; i++) {
            auto mirror = mirrors_[i];
            if (mirror->Status == ImTextureStatus_OK) {
                // Pending updates were uploaded by the render thread
                mirror->Updates.resize(0);
                mirror->UpdateRect.x = mirror->UpdateRect.y = (unsigned short)~0;
                mirror->UpdateRect.w = mirror->UpdateRect.h = 0;
            } else if (mirror->Status == ImTextureStatus_Destroyed) {
                IM_DELETE(mirror);
                mirrors_.erase(mirrors_.begin() + i);
                i--;
            }
        }
    }

    void SyncTexture(ImTextureData* tex)
    {
        auto mirror = static_cast<ImTextureData*>(tex->QueueUserData);

        switch (tex->Status) {
        case ImTextureStatus_WantCreate:
            if (mirror != nullptr) {
                RetireMirror(mirror);
            }

            mirror = IM_NEW(ImTextureData)();
            mirror->UniqueID = tex->UniqueID;
            mirror->Create(tex->Format, tex->Width, tex->Height);
            memcpy(mirror->Pixels, tex->Pixels, tex->GetSizeInBytes());
            mirrors_.push_back(mirror);

            tex->QueueUserData = mirror;
            tex->SetStatus(ImTextureStatus_OK);
            break;

        case ImTextureStatus_WantUpdates:
            for (auto const& r : tex->Updates) {
                ImFontAtlasTextureBlockCopy(tex, r.x, r.y, mirror, r.x, r.y, r.w, r.h);
                ImTextureDataQueueUpload(mirror, r.x, r.y, r.w, r.h);
            }

            tex->SetStatus(ImTextureStatus_OK);
            break;

        case ImTextureStatus_WantDestroy:
            if (mirror != nullptr) {
                RetireMirror(mirror);
                // Lets IMGUI finish destroying its own copy
                tex->QueueUserData = nullptr;
            }
            break;

        default:
            break;
        }
    }

    static void RetireMirror(ImTextureData* mirror)
    {
        // WantDestroyNextFrame prevents SetStatus(Destroyed) from turning the destroyed mirror into a create request
        mirror->WantDestroyNextFrame = true;
        mirror->Status = ImTextureStatus_WantDestroy;
        mirror->UnusedFrames = 0;
    }
};

END_NS()
