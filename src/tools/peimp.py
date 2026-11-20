# peimp.py - check that a PE file's imports are exported by DLLs in the same folder.
# usage: python -I peimp.py <exe|dll|ocx path>
# Error 127 (ERROR_PROC_NOT_FOUND) diagnosis: prints MISSING functions per DLL.

import struct, sys, os

def load(path):
    d = open(path, 'rb').read()
    pe = struct.unpack_from('<I', d, 0x3c)[0]
    nsec = struct.unpack_from('<H', d, pe + 6)[0]
    optsz = struct.unpack_from('<H', d, pe + 20)[0]
    opt = pe + 24
    magic = struct.unpack_from('<H', d, opt)[0]
    ddir = opt + (96 if magic == 0x10b else 112)
    secs = []
    s = opt + optsz
    for i in range(nsec):
        secs.append((struct.unpack_from('<I', d, s + 12)[0], struct.unpack_from('<I', d, s + 8)[0], struct.unpack_from('<I', d, s + 20)[0], struct.unpack_from('<I', d, s + 16)[0]))
        s += 40
    def off(rva):
        for va, vsz, raw, rsz in secs:
            if va <= rva < va + max(vsz, rsz):
                return rva - va + raw
        return None
    def cstr(o):
        e = d.index(b'\0', o)
        return d[o:e].decode('latin1')
    return d, ddir, off, cstr, magic

def exports(path):
    d, ddir, off, cstr, magic = load(path)
    rva = struct.unpack_from('<I', d, ddir)[0]
    if not rva:
        return set()
    o = off(rva)
    nnames = struct.unpack_from('<I', d, o + 24)[0]
    pnames = off(struct.unpack_from('<I', d, o + 32)[0])
    res = set()
    for i in range(nnames):
        res.add(cstr(off(struct.unpack_from('<I', d, pnames + 4 * i)[0])))
    return res

def imports(path):
    d, ddir, off, cstr, magic = load(path)
    rva = struct.unpack_from('<I', d, ddir + 8)[0]
    o = off(rva)
    res = {}
    while True:
        oft, ts, fc, name, ft = struct.unpack_from('<IIIII', d, o)
        if name == 0:
            break
        dll = cstr(off(name))
        t = off(oft or ft)
        names = []
        while True:
            v = struct.unpack_from('<I', d, t)[0]
            if v == 0:
                break
            if v & 0x80000000:
                names.append('#%d' % (v & 0xffff))
            else:
                names.append(cstr(off(v) + 2))
            t += 4
        res[dll] = names
        o += 20
    return res

target = sys.argv[1]
folder = os.path.dirname(target)
imp = imports(target)
for dll, names in imp.items():
    p = os.path.join(folder, dll)
    if not os.path.exists(p):
        continue
    ex = exports(p)
    missing = [n for n in names if not n.startswith('#') and n not in ex]
    print('%-20s imports=%d missing=%d' % (dll, len(names), len(missing)))
    for n in missing:
        print('    MISSING', n)
