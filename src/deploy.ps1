param (
    [string]$TargetPath,
    [string]$ProjectDir
)

# Find dllmain.cpp to extract the addon name
$dllMainPath = Join-Path $ProjectDir "dllmain.cpp"
$addonName = "addon"

if (Test-Path $dllMainPath) {
    $content = Get-Content $dllMainPath -Raw
    if ($content -match 'AddonDef\.Name\s*=\s*"([^"]+)"') {
        $addonName = $matches[1] -replace '\s+', '_'
    }
}

$regPath = Get-ItemProperty "HKLM:\SOFTWARE\ArenaNet\Guild Wars 2" -ErrorAction SilentlyContinue
if (-not $regPath) {
    $regPath = Get-ItemProperty "HKCU:\SOFTWARE\ArenaNet\Guild Wars 2" -ErrorAction SilentlyContinue
}

if ($regPath -and $regPath.Path) {
    $gw2exe = $regPath.Path
    $gw2dir = Split-Path $gw2exe
    $addonsDir = Join-Path $gw2dir "addons"
    
    if (-not (Test-Path $addonsDir)) {
        New-Item -ItemType Directory -Path $addonsDir | Out-Null
    }
    
    $ext = [System.IO.Path]::GetExtension($TargetPath)
    $destPath = Join-Path $addonsDir ($addonName + $ext)
    
    Write-Host "Deploying addon to $destPath..."
    Copy-Item -Path $TargetPath -Destination $destPath -Force
    Write-Host "Deployment successful."
} else {
    Write-Host "Guild Wars 2 installation not found in registry. Skipping deployment."
}
