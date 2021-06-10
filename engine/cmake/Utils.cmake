##########################################
#
# Maze Engine
# Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented;
#    you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment
#    in the product documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
#
##########################################


##########################################
function(append_linker_flags _target _flags)
    set(new_link_flags ${_flags})
    get_target_property(existing_link_flags ${_target} LINK_FLAGS)
    if(existing_link_flags)
        set(new_link_flags "${existing_link_flags} ${new_link_flags}")
    endif()
    set_target_properties(${_target} PROPERTIES LINK_FLAGS ${new_link_flags})
endfunction()


##########################################
function(append_linker_flags_config _target _config _flags)
    set(new_link_flags ${_flags})
    get_target_property(existing_link_flags ${_target} "LINK_FLAGS_${_config}")
    if(existing_link_flags)
        set(new_link_flags "${existing_link_flags} ${new_link_flags}")
    endif()
    set_target_properties(${_target} PROPERTIES "LINK_FLAGS_${_config}" ${new_link_flags})
endfunction()


##########################################
function(string_starts_with _str _search _result)
  string(FIND "${_str}" "${_search}" out)
  if("${out}" EQUAL 0)
    set(${_result} TRUE PARENT_SCOPE)
  else()
    set(${_result} FALSE PARENT_SCOPE)
    endif()
endfunction()