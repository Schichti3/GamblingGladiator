[CmdletBinding()]
param(
    [string]$Msys2Root = "C:\msys64"
)

$ErrorActionPreference = "Stop"

function Invoke-Checked {
    param(
        [Parameter(Mandatory)]
        [string]$FilePath,

        [Parameter(Mandatory)]
        [string[]]$Arguments
    )

    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code ${LASTEXITCODE}: $FilePath $($Arguments -join ' ')"
    }
}

$Msys2Root = [System.IO.Path]::GetFullPath($Msys2Root)
$pacman = Join-Path $Msys2Root "usr\bin\pacman.exe"

if (-not (Test-Path -LiteralPath $pacman)) {
    $winget = Get-Command winget.exe -ErrorAction SilentlyContinue
    if (-not $winget) {
        throw "winget.exe is required to install MSYS2. Install App Installer, then run this script again."
    }

    Write-Host "Installing MSYS2 in $Msys2Root..."
    Invoke-Checked $winget.Source @(
        "install",
        "--id", "MSYS2.MSYS2",
        "--exact",
        "--silent",
        "--location", $Msys2Root,
        "--accept-package-agreements",
        "--accept-source-agreements",
        "--disable-interactivity"
    )

    if (-not (Test-Path -LiteralPath $pacman)) {
        throw "MSYS2 was installed, but pacman.exe was not found at $pacman. Re-run with -Msys2Root pointing to the installation directory."
    }
} else {
    Write-Host "Using existing MSYS2 installation at $Msys2Root."
}

[Environment]::SetEnvironmentVariable("MSYS2_ROOT", $Msys2Root, "User")
$env:MSYS2_ROOT = $Msys2Root

Write-Host "Updating MSYS2 packages..."
Invoke-Checked $pacman @("--noconfirm", "-Syu")

$packages = @(
    "mingw-w64-ucrt-x86_64-gcc",
    "mingw-w64-ucrt-x86_64-pkgconf",
    "mingw-w64-ucrt-x86_64-cmake",
    "mingw-w64-ucrt-x86_64-ninja",
    "mingw-w64-ucrt-x86_64-raylib",
    "mingw-w64-ucrt-x86_64-enet"
)

Write-Host "Installing UCRT64 build dependencies..."
Invoke-Checked $pacman (@("--noconfirm", "--needed", "-S") + $packages)

$ucrtBin = Join-Path $Msys2Root "ucrt64\bin"
$env:Path = "$ucrtBin;$env:Path"

Write-Host "MSYS2 setup complete. MSYS2_ROOT is set to $Msys2Root."
Write-Host "Open a new terminal before building with the mingw-gcc-ucrt presets."
