aws s3 cp C:\Dev\LS\BG3PublishingRoot\Devel\GameManifest.json s3://nb-stor/bg3-updater-v3/Channels/Devel/Manifest.json
aws s3api put-object-acl --bucket nb-stor --key bg3-updater-v3/Channels/Devel/Manifest.json --acl public-read
aws cloudfront create-invalidation --distribution-id E300IVONTDEMJX --paths /Channels/Devel/Manifest.json

aws s3 cp C:\Dev\LS\BG3PublishingRoot\Release\GameManifest.json s3://nb-stor/bg3-updater-v3/Channels/Release/Manifest.json
aws s3api put-object-acl --bucket nb-stor --key bg3-updater-v3/Channels/Release/Manifest.json --acl public-read
aws cloudfront create-invalidation --distribution-id E300IVONTDEMJX --paths /Channels/Release/Manifest.json
pause
