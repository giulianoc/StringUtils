
# Copyright (C) Giuliano Catrambone (giulianocatrambone@gmail.com)

# This program is free software; you can redistribute it and/or 
# modify it under the terms of the GNU General Public License 
# as published by the Free Software Foundation; either 
# version 2 of the License, or (at your option) any later 
# version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# Commercial use other than under the terms of the GNU General Public
# License is allowed only after express negotiation of conditions
# with the authors.

SET (SOURCES
	StringUtils.cpp
)

SET (HEADERS
	StringUtils.h
)

if(STRINGUTILS_UTFCPP)
	add_compile_options(-DUTFCPP)
  include_directories("${UTFCPP_INCLUDE_DIR}")
endif()

add_library (StringUtils SHARED ${SOURCES} ${HEADERS})

if(ZORAC)
	install (TARGETS StringUtils DESTINATION services/cms-import)
	install (TARGETS StringUtils DESTINATION services/cms-pusher)
	install (TARGETS StringUtils DESTINATION services/cms-getter)
else()
	install (TARGETS StringUtils DESTINATION lib)
	install (FILES ${HEADERS} DESTINATION include)
endif()

