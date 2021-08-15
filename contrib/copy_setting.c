/* ----------------------------------------------------------------------------
   libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2018  Mark A Lindner

   This file is part of libconfig.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, see
   <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

static void config_setting_copy_simple(config_setting_t * parent, const config_setting_t * src);
static void config_setting_copy_elem(config_setting_t * parent, const config_setting_t * src);

static void config_setting_copy_aggregate(config_setting_t * parent, const config_setting_t * src);
static int config_setting_copy(config_setting_t * parent, const config_setting_t * src);

static void config_setting_copy_simple(config_setting_t * parent, const config_setting_t * src)
{
    if(config_setting_is_aggregate(src))
    {
        config_setting_copy_aggregate(parent, src);
    }
    else 
    {
        config_setting_t * set;
        
        set = config_setting_add(parent, config_setting_name(src), config_setting_type(src));

        if(CONFIG_TYPE_INT == config_setting_type(src))
        {
            config_setting_set_int(set, config_setting_get_int(src));
            config_setting_set_format(set, src->format);
        }
        else if(CONFIG_TYPE_INT64 == config_setting_type(src))
        {
            config_setting_set_int64(set, config_setting_get_int64(src));
            config_setting_set_format(set, src->format);
        }
        else if(CONFIG_TYPE_FLOAT == config_setting_type(src))
            config_setting_set_float(set, config_setting_get_float(src));
        else if(CONFIG_TYPE_STRING == config_setting_type(src))
            config_setting_set_string(set, config_setting_get_string(src));
        else if(CONFIG_TYPE_BOOL == config_setting_type(src))
            config_setting_set_bool(set, config_setting_get_bool(src));
    }
}

static void config_setting_copy_elem(config_setting_t * parent, const config_setting_t * src)
{
    config_setting_t * set;
    
    set = NULL;
    if(config_setting_is_aggregate(src))
        config_setting_copy_aggregate(parent, src);
    else if(CONFIG_TYPE_INT == config_setting_type(src))
    {
        set = config_setting_set_int_elem(parent, -1, config_setting_get_int(src));
        config_setting_set_format(set, src->format);
    }
    else if(CONFIG_TYPE_INT64 == config_setting_type(src))
    {
        set = config_setting_set_int64_elem(parent, -1, config_setting_get_int64(src));
        config_setting_set_format(set, src->format);   
    }
    else if(CONFIG_TYPE_FLOAT == config_setting_type(src))
        set = config_setting_set_float_elem(parent, -1, config_setting_get_float(src));
    else if(CONFIG_TYPE_STRING == config_setting_type(src))
        set = config_setting_set_string_elem(parent, -1, config_setting_get_string(src));
    else if(CONFIG_TYPE_BOOL == config_setting_type(src))
        set = config_setting_set_bool_elem(parent, -1, config_setting_get_bool(src));
}

static void config_setting_copy_aggregate(config_setting_t * parent, const config_setting_t * src)
{
    config_setting_t * newAgg;
    int i, n;

    newAgg = config_setting_add(parent, config_setting_name(src), config_setting_type(src));
    
    n = config_setting_length(src);    
    for(i = 0; i < n; i++)
    {
        if(config_setting_is_group(src))
        {
            config_setting_copy_simple(newAgg, config_setting_get_elem(src, i));            
        }
        else
        {
            config_setting_copy_elem(newAgg, config_setting_get_elem(src, i));
        }        
    }
}

static int config_setting_copy(config_setting_t * parent, const config_setting_t * src)
{
    if((!config_setting_is_group(parent)) &&
       (!config_setting_is_list(parent)))
        return CONFIG_FALSE;

    if(config_setting_is_aggregate(src))
    {
        config_setting_copy_aggregate(parent, src);
    }
    else
    {
        config_setting_copy_simple(parent, src);
    }
    
    return CONFIG_TRUE;
}


//Some sample code

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) 
//-----------------------------------------------------------------------------
{
    config_t cfgSrc;
    config_t cfgSrcCopy;
    config_t cfgDst;
    

    config_init(&cfgSrc);
    config_init(&cfgSrcCopy);
    config_init(&cfgDst);
    
    if(CONFIG_FALSE == config_read_file(&cfgSrc, "/data/menu/cfgSrc.cfg"))
    {
        fprintf(stderr, "Failed to open cfgSrc.cfg\n");
    }
    if(CONFIG_FALSE == config_read_file(&cfgDst, "/data/menu/cfgDst.cfg"))
    {
        fprintf(stderr, "Failed to open cfgDst.cfg\n");
    }    
    
    /*
    printf("Dump cfgSrc.cfg\n");
    DumpCfgSetting(config_root_setting(&cfgSrc));
    
    printf("Dump cfgDst.cfg\n");
    DumpCfgSetting(config_root_setting(&cfgDst));
    */

    config_setting_t * src;
    config_setting_t * dst;

    dst = config_lookup(&cfgDst, "grp1");

    if((0 != (dst = config_lookup(&cfgDst, "grp1"))) &&
       (0 != (src = config_lookup(&cfgSrc, "application.window"))))
       //(0 != (src = config_lookup(&cfgSrc, "list"))))
       
    {
        if(CONFIG_FALSE == config_setting_copy(dst, src))
        {
            printf("Failed to copy src to dst\n");
        }
    }

    config_setting_copy(config_root_setting(&cfgSrcCopy), config_root_setting(&cfgSrc));

    config_write_file(&cfgDst, "/data/menu/cfgDstMod.cfg");
    config_write_file(&cfgSrcCopy, "/data/menu/cfgSrcCpy.cfg");
    config_write_file(&cfgSrc, "/data/menu/cfgSrcOrig.cfg");

    config_destroy(&cfgSrc);
    config_destroy(&cfgDst);

    return 0;
}
