/* libcomps - C alternative to yum.comps library
 * Copyright (C) 2013 Jindrich Luza
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to  Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA
 */

#include "comps_docpackage.h"
void comps_docpackage_create(COMPS_DocGroupPackage* package, COMPS_Object **args) {

    (void)args;
    package->name = NULL;
    package->requires = NULL;
}
COMPS_CREATE_u(docpackage, COMPS_DocGroupPackage)

void comps_docpackage_copy(COMPS_DocGroupPackage *pkg_dst,
                           COMPS_DocGroupPackage *pkg_src) {
    pkg_dst->name = (COMPS_Str*)comps_object_copy((COMPS_Object*)pkg_src->name);
    pkg_dst->requires = (COMPS_Str*)comps_object_copy(
                                            (COMPS_Object*)pkg_src->requires);
    pkg_dst->type = pkg_src->type;
}
COMPS_COPY_u(docpackage, COMPS_DocGroupPackage)    /*comps_utils.h macro*/

void comps_docpackage_destroy(COMPS_DocGroupPackage *pkg) {
    comps_object_destroy((COMPS_Object*)pkg->name);
    comps_object_destroy((COMPS_Object*)pkg->requires);
}
COMPS_DESTROY_u(docpackage, COMPS_DocGroupPackage) /*comps_utils.h macro*/


void comps_docpackage_set_name(COMPS_DocGroupPackage *pkg, char *name) {
    if (pkg->name)
        comps_object_destroy((COMPS_Object*)pkg->name);
    pkg->name = comps_str(name);
}
void comps_docpackage_set_requires(COMPS_DocGroupPackage *pkg, char *requires) {
    if (pkg->requires)
        comps_object_destroy((COMPS_Object*)pkg->requires);
    pkg->requires = comps_str(requires);
}
void comps_docpackage_set_type(COMPS_DocGroupPackage *pkg,
                                   COMPS_PackageType type) {
    pkg->type = type;
}


signed char comps_docpackage_cmp_u(COMPS_Object *pkg1, COMPS_Object *pkg2) {
    #define _pkg1 ((COMPS_DocGroupPackage*)pkg1)
    #define _pkg2 ((COMPS_DocGroupPackage*)pkg2)

    if (!comps_object_cmp((COMPS_Object*)_pkg1->name,
                          (COMPS_Object*)_pkg2->name)) return 0;
    if (!comps_object_cmp((COMPS_Object*)_pkg1->requires,
                          (COMPS_Object*)_pkg2->requires)) return 0;
    if (_pkg1->type != _pkg2->type) return 0;
    return 1;

    #undef _pkg1
    #undef _pkg2
}
inline char comps_docpackage_cmp_set(void *pkg1, void *pkg2) {
    return comps_docpackage_cmp_u((COMPS_Object*) pkg1, (COMPS_Object*)pkg2);
}

void comps_docpackage_xml(COMPS_DocGroupPackage *pkg,
                          xmlTextWriterPtr writer,
                          COMPS_Logger *log) {
    char *str;

    xmlTextWriterStartElement(writer, BAD_CAST "packagereq");
    if (pkg->type == COMPS_PACKAGE_OPTIONAL)
        str = "optional";
    else if (pkg->type == COMPS_PACKAGE_MANDATORY)
        str = "mandatory";
    else if (pkg->type == COMPS_PACKAGE_CONDITIONAL)
        str = "conditional";
    else
        str = "default";
    xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST str);
    if (pkg->requires) {
        str = comps_object_tostr((COMPS_Object*)pkg->requires);
        xmlTextWriterWriteAttribute(writer, (xmlChar*) "requires",
                                            BAD_CAST str);
        free(str);
    }
    str = comps_object_tostr((COMPS_Object*)pkg->name);
    xmlTextWriterWriteString(writer, (xmlChar*)str);
    free(str);
    xmlTextWriterEndElement(writer);
}

COMPS_ObjectInfo COMPS_DocGroupPackage_ObjInfo = {
    .obj_size = sizeof(COMPS_DocGroupPackage),
    .constructor = &comps_docpackage_create_u,
    .destructor = &comps_docpackage_destroy_u,
    .copy = &comps_docpackage_copy_u,
    .obj_cmp = &comps_docpackage_cmp_u
};