param (
    [string]$Python = ""
)

$ScriptDirectory = Split-Path -parent $PSCommandPath

if (Test-Path $Python) {
    $EMSDK_PY = $Python
}

# As a last resort, access from PATH.
if (-Not $EMSDK_PY) {
    $EMSDK_PY = "python"
}

# Tell EMSDK to create environment variable setter as a .ps1 file
$env:EMSDK_POWERSHELL = 1

& $EMSDK_PY "$ScriptDirectory\emsdk\emsdk" $args

# python is not able to set environment variables to the parent calling process, so
# therefore have it craft a .ps1 file, which we invoke after finishing python execution,
# to set up the environment variables
if (Test-Path ./emsdk/emsdk_set_env.ps1) {
    & ./emsdk/emsdk_set_env.ps1
    Remove-Item ./emsdk/emsdk_set_env.ps1
}

Remove-Item Env:\EMSDK_POWERSHELL
