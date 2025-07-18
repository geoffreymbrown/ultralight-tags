[#ftl]
[#--
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,
              2011,2012,2013,2014,2015,2016,2017 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[@pp.changeOutputFile name="board.mk" /]
[#if doc1.board.configuration_settings.board_files_path[0]??]
  [#assign path = doc1.board.configuration_settings.board_files_path[0]?string?trim /]
  [#if !path?ends_with("/")]
    [#assign path = path + "/"]
  [/#if]
[#else]
  [#if doc1.board.configuration_settings.hal_version[0]?trim == "2.6.x"]
    [#assign path = "$(CHIBIOS)/boards/" /]
  [#else]
    [#assign path = "$(CHIBIOS)/os/hal/boards/" /]
  [/#if]
[/#if]
# List of all the board related files.

BOARDSRC = $(BOARDDIR)/BitPresTagv1/board.c

# Required include directories
BOARDINC = $(BOARDDIR)/BitPresTagv1/

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
