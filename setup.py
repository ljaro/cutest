#!/usr/bin/env python

from distutils.core import setup

setup(name='cutestpy',
      version='0.1',
      description='Library for testing QT GUI applications',
      author=['ljaro', 'van9konst'],
      license="GNU Lesser General Public License v3.0",
      email="luk8dev@gmail.com",
      url='https://github.com/ljaro/cutest',
      packages=['cutestpy'], requires=['pytest']  # , 'teamcity-messages']
      )
