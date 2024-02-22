@@findstr/v "^@@.*" "%~f0" | powershell -NonInteractive -NoProfile -ExecutionPolicy Bypass - 
@@pause & goto:eof
# The above lines allows running this file as a batch file, to overcome the
# need for execution policy changes.

# Note, when making changes to this script, testing needs to be done under the
# version of PowerShell that comes with Windows (PowerShell v5), as that is
# what most users will have access to.

# ## Constants ## #
$OUT_FILE_PATH     = "${env:temp}\BG3 Diagnostic Data.txt"
$OUT_FILE_ENCODING = "utf8"
$OUT_FILE_WIDTH    = ([int]::MaxValue - 1)

$BG3_GOG_DIRECTORY   = "C:\Program Files (x86)\GOG Galaxy\Games\Baldurs Gate 3"
$BG3_STEAM_DIRECTORY = "C:\Program Files (x86)\Steam\steamapps\common\Baldurs Gate 3"
$BG3_STATE_DIRECTORY = "~\AppData\Local\Larian Studios\Baldur's Gate 3"

$SECTION_START_PREFIX   = "Start Section: "
$SECTION_END_PREFIX     = "End Section: "
$SECTION_UNDERLINE_CHAR = "="
$SECTION_INDENT         = "    "


# ## Defaults ## #
$dpv = $PSDefaultParameterValues

# Set defaults for the Out-File cmdlet.
$dpv.Add("Out-File:FilePath", $OUT_FILE_PATH)
$dpv.Add("Out-File:Encoding", $OUT_FILE_ENCODING)
$dpv.Add("Out-File:Width",    $OUT_FILE_WIDTH)
$dpv.Add("Out-File:Append",   $true)

# Out-String will pad output to `width` before stripping padding away, causing
# out-of-memory errors if [int]::MaxValue is used.
$dpv.Add("Out-String:Width",  2048)

# Specifying a "+" in front of the name for ErrorVariable will add the errors
# to the variable rather than replace them.
$dpv.Add("*:ErrorVariable", "+errors")


# ## Functions ## #
$SECTION_LEVEL = 0

function Invoke-Section {
    param(
        [Parameter(Mandatory=$true)]
        [String]
        $Name,

        [Parameter(Mandatory=$true)]
        [ScriptBlock]
        $ScriptBlock
    )

    # Calculate the section names
    $SectionStart          = "${SECTION_START_PREFIX}${Name}"
    $SectionEnd            = "${SECTION_END_PREFIX}${Name}"
    $SectionStartUnderline = $SECTION_UNDERLINE_CHAR * $SectionStart.Length
    $SectionEndUnderline   = $SECTION_UNDERLINE_CHAR * $SectionEnd.Length

    # Encapsulate our main logic for the conditional Write-Output call.
    $logic = {
      $SECTION_LEVEL = $SECTION_LEVEL + 1
      Write-Output "$SectionStart"
      Write-Output "$SectionStartUnderline"

      Invoke-Command $ScriptBlock             |
        Out-String -Stream       |
        Foreach-Object {
          $_ -replace "^","$SECTION_INDENT"
        }

      Write-Output "$SectionEndUnderline"
      Write-Output "$SectionEnd"
      Write-Output ""
      Write-Output ""
      $SECTION_LEVEL = $SECTION_LEVEL - 1
    }

    # We only want to call Write-Output if we are the top-level section.
    if ($SECTION_LEVEL -eq 0) {
      Invoke-Command $logic | Out-File
    } else {
      Invoke-Command $logic
    }
}


function Fit-ViewSize() {

}


# ## Clear/Create Output File ## #
Set-Content $OUT_FILE_PATH ""


# ## Get Basic System Information ## #
Invoke-Section "Basic System Information" {
  $PSVersionTable | Format-Table
}

# ## Generate File Hashes ## #
Invoke-Section "File Hashes" {
  $inputPaths = @(
    $BG3_GOG_DIRECTORY,
    $BG3_STEAM_DIRECTORY,
    $BG3_STATE_DIRECTORY
  )

  $inputPaths                                        | `
    Where-Object { Test-Path $_ }                    | `
    Get-ChildItem -File -Recurse                     | `
    Where-Object FullName -notlike "*.log"           | `
    Where-Object FullName -notlike "*.pak"           | `
    Where-Object FullName -notlike "*LevelCache*"    | `
    Where-Object FullName -notlike "*Savegames*"     | `
    Where-Object FullName -notlike "*DigitalDeluxe*" | `
    Where-Object FullName -notlike "*Shaders*"       | `
    Where-Object FullName -notlike "*Stats*"         | `
    Foreach-Object {
        Write-Host $_.FullName
        Get-FileHash       `
          -Algorithm   MD5 `
          -LiteralPath $_.FullName
    }                                               | `
    Sort-Object  -Property Path                     | `
    Format-Table -AutoSize
}


# ## Get modsettings.lsx contents ## #
Invoke-Section "Mod Settings Files" {
  $BG3_STATE_DIRECTORY        | `
    Get-ChildItem               `
      -File                     `
      -Recurse                  `
      -Filter modsettings.lsx | `
    Foreach-Object {
      Write-Host $_.FullName
      Invoke-Section            `
        -Name $_.FullName       `
        -ScriptBlock {
          Get-Content $_.FullName
        }
    }
}


# ## Log Errors ## #
# Since we can't make any assumptions about the current execution state here,
# we avoid using functions and logic defined elsewhere as much as possible.
$errors | Foreach-Object {
  begin {
    Write-Output "Errors"
    Write-Output "======"
  }
  process {
    Write-Output "$_"
  }
  end {
    Write-Output "======"
    Write-Output "Errors"
  }
} | Out-File


# ## Finishing Up ## #
if (Test-Path $OUT_FILE_PATH) {
  Start-Process explorer -ArgumentList "/select,`"$OUT_FILE_PATH`""
}
Write-Host "An output file should be present at `"$OUT_FILE_PATH`"."
Write-Host "If it not present, check that Baldur's Gate 3 is not currently running, and run this script again."
Write-Host "Otherwise, this window can be closed."

# For whatever strange reason, our method to run this file via cmd.exe
# requires exactly zero blank lines after this comment, otherwise execution
# of code near the end of the file is skipped. This is probably due to
# weirdness regarding line-by-line execution and how unread input from
# Powershell is passed to the `pause` command when Powershell terminates.
