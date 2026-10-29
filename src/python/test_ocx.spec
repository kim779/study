# -*- mode: python ; coding: utf-8 -*-


a = Analysis(
    ['test_ocx.py'],
    pathex=[],
    binaries=[],
    datas=[],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

# onedir: exe + _internal\ folder (no temp extraction at startup).
# Deploy both into <home>\exe\ - axisver relaunch and HTS DLL lookup rely on the exe living there.
exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='IBKs_openAPI_python',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    console=False,
    uac_admin=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
coll = COLLECT(
    exe,
    a.binaries,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='IBKs_openAPI_python',
)
