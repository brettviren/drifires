#!/usr/bin/env waf

import os.path as osp

import subprocess

VERSION='0.0.0'
APPNAME='drifires'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx')

    opt.add_option('--with-nljs', default=None,
                   help='Point to nlohmann::json install area')
    opt.add_option('--with-root', default=None,
                   help='Point to ROOT install area')
    opt.add_option('--with-garfieldpp', default=None,
                   help='Point to Garfield++ install area')
    opt.add_option('--with-moo-exec', default='moo',
                   help='Set moo program')

def have_lib(cfg, name, header, libs):
    NAME = name.upper()
    have = getattr(cfg.options, 'with_'+name, None)
    if have:
        print("using %s at %s" % (name, have))
        setattr(cfg.env, 'RPATH_'+NAME, [osp.join(have, 'lib')]);
        setattr(cfg.env, 'LIBPATH_'+NAME, [osp.join(have, 'lib')]);
        setattr(cfg.env, 'INCLUDES_'+NAME, [osp.join(have, 'include')]);
    cfg.check(features='cxx cxxprogram', define_name='HAVE_'+NAME,
              header=header, lib=libs,
              use=NAME, uselib_store=NAME, mandatory=True)    


def have_hlib(cfg, name, header):
    NAME=name.upper()
    have = getattr(cfg.options, 'with_'+name, None)
    if have:
        print("using %s at %s" % (name, have))
        setattr(cfg.env, 'INCLUDES_'+NAME, [osp.join(have, "include")])

    cfg.check(features='cxx cxxprogram', define_name='HAVE_'+NAME,
              header_name=header,
              use=NAME, uselib_store=NAME, mandatory=True)

def configure(cfg):
    cfg.env.CXXFLAGS += ['-std=c++17', '-ggdb3',
                         '-Wall', '-Wpedantic', '-Werror']
    cfg.load('compiler_cxx')


    have_hlib(cfg, 'nljs', 'nlohmann/json.hpp')
    have_lib(cfg, 'garfieldpp', 'Garfield/Medium.hh', ['Garfield'])
    have_lib(cfg, 'root', 'Rtypes.h', [
        'Core', 'Graf', 'Hist', 'Graf3d', 'Gpad', 'Geom', 'Matrix', 'MathCore'])

    cfg.find_program(cfg.options.with_moo_exec,
                     var='MOO', define_name='HAVE_MOO', mandatory=False)

    cfg.env.DO_CODEGEN = False
    if cfg.env.MOO:
        cfg.env.DO_CODEGEN = True
        

def build(bld):
    uses='NLJS GARFIELDPP ROOT'.split()


    if bld.env.DO_CODEGEN:
        print("codegen")
        incdir = bld.path.find_dir("inc/drifires")
        codegen = bld.path.find_resource("src/drifires-codegen.jsonnet")
        imports = bld.env.MOO[0] + " imports " + codegen.abspath()
        imports = subprocess.check_output(imports, shell=True)
        imports = imports.decode().split("\n")
        imports = [bld.root.find_node(im) for im in imports]
        name = codegen.name.replace("-codegen.jsonnet","")
        for ttype in ["nljs", "structs"]:
            tmpl = bld.path.find_resource('src/'+ttype+'.hpp.j2')
            assert(tmpl)
            print(codegen,tmpl)
            tgt = incdir.make_node("%s.hpp" % ttype)
            bld(name = 'codegen_' + ttype,
                rule="${MOO} render -o ${TGT} ${SRC[0].abspath()} ${SRC[1].abspath()}",
                source=[codegen,tmpl]+imports, target=[tgt])
        bld.add_group()         # want this to finish first 
    

    rpath = [bld.env["PREFIX"] + '/lib',
             bld.path.find_or_declare(bld.out_dir)]
    #rpath += [bld.env["LIBPATH_%s"%u][0] for u in uses]
    rpath = list(set(rpath))    

    sources = bld.path.ant_glob('src/*.cpp')

    bld.shlib(features='cxx', includes='inc', 
              source = sources, target='drifires',
              uselib_store='DRIFIRES', use=uses)

    bld.install_files('${PREFIX}/include/drifires',
                      bld.path.ant_glob("inc/drifires/*.hpp"),
                      cwd=bld.path.find_dir('inc/drifires'),
                      install_path=bld.env.PREFIX + '/lib',
                      relative_trick=True)                      

    for one in bld.path.ant_glob('apps/*.cpp'):
        bld.program(features = 'cxx',
                    includes = '../inc .',
                    source = [one],
                    target = one.name.replace('.cpp',''),
                    rpath = rpath,
                    use = ['drifires'] + uses)

    bld.program(features = 'cxx',
                includes = '../inc .',
                source = bld.path.ant_glob('test/test_*.cpp'),
                target = 'drifires-test',
                rpath = rpath,
                use = ['drifires'] + uses)

