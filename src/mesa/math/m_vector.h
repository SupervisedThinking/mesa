/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * New (3.1) transformation code written by Keith Whitwell.
 */


#ifndef _M_VECTOR_H_
#define _M_VECTOR_H_

#include "util/glheader.h"
#define MATH_ASM_PTR_SIZE sizeof(void *)
#include "math/m_vector_asm.h"

#define VEC_MALLOC         0x10 /* storage field points to self-allocated mem*/
#define VEC_NOT_WRITEABLE  0x40	/* writable elements to hold clipped data */
#define VEC_BAD_STRIDE     0x100 /* matches tnl's prefered stride */





/**
 * Wrap all the information about vectors up in a struct.  Has
 * additional fields compared to the other vectors to help us track
 * different vertex sizes, and whether we need to clean columns out
 * because they contain non-(0,0,0,1) values.
 *
 * The start field is used to reserve data for copied vertices at the
 * end of _mesa_transform_vb, and avoids the need for a multiplication in
 * the transformation routines.
 */
typedef struct {
   GLfloat (*data)[4];	/**< may be malloc'd or point to client data */
   GLfloat *start;	/**< points somewhere inside of GLvector4f::data */
   GLuint count;	/**< size of the vector (in elements) */
   GLuint stride;	/**< stride from one element to the next (in bytes) */
   GLuint size;		/**< 2-4 for vertices and 1-4 for texcoords */
   GLbitfield flags;	/**< bitmask of VEC_x flags */
   void *storage;	/**< self-allocated storage */
   GLuint storage_count; /**< storage size in elements */
} GLvector4f;


extern void _mesa_vector4f_init( GLvector4f *v, GLbitfield flags,
			      GLfloat (*storage)[4] );
extern void _mesa_vector4f_alloc( GLvector4f *v, GLbitfield flags,
			       GLuint count, GLuint alignment );
extern void _mesa_vector4f_free( GLvector4f *v );
extern void _mesa_vector4f_print( const GLvector4f *v, const GLubyte *, GLboolean );
extern void _mesa_vector4f_clean_elem( GLvector4f *vec, GLuint nr, GLuint elt );


/**
 * Given vector <v>, return a pointer (cast to <type *> to the <i>-th element.
 *
 * End up doing a lot of slow imuls if not careful.
 */
#define VEC_ELT( v, type, i ) \
       ( (type *)  ( ((GLbyte *) ((v)->data)) + (i) * (v)->stride) )


#endif
