#!/usr/bin/env waf

import os.path as osp

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
    cfg.env.CXXFLAGS += ['-std=c++17', '-ggdb3', '-Wall', '-Wpedantic', '-Werror']
    cfg.load('compiler_cxx')


    have_hlib(cfg, 'nljs', 'nlohmann/json.hpp')
    have_lib(cfg, 'garfieldpp', 'Garfield/Medium.hh', ['Garfield'])
    have_lib(cfg, 'root', 'Rtypes.h', ['Core', 'Graf', 'Hist', 'Graf3d', 'Gpad', 'Geom', 'Matrix', 'MathCore'])



def build(bld):
    uses='NLJS GARFIELDPP ROOT'.split()

    sources = bld.path.ant_glob('src/*.cpp')
    bld.shlib(features='cxx', includes='inc', 
              source = sources, target='drifires',
              uselib_store='DRIFIRES', use=uses)

    bld.install_files('${PREFIX}/include/drifires',
                      bld.path.ant_glob("inc/drifires/**/*.hpp"),
                      cwd=bld.path.find_dir('inc/drifires'),
                      install_path=bld.env.PREFIX + '/lib',
                      relative_trick=True)                      
    for one in bld.path.ant_glob('apps/*.cpp'):
        print (one.name)
        bld.program(features = 'cxx',
                    includes = '../inc .',
                    source = [one],
                    target = one.name.replace('.cpp',''),
                    rpath = [bld.env.LIBDIR],
                    use = uses)
