/*
 * Mode.h
 * Andrew Sayman - 10/11/04
 *
 * Copyright (C) 2004  Andrew Sayman
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#ifndef MODE_H
#define MODE_H

// meta game modes
#define CM_SERVER                        (1 << 0)
#define CM_CLIENT                        (1 << 1)
#define CM_SOLO                          (1 << 2)
#define CM_AI                            (1 << 3)
#define CM_X                             (1 << 4)
#define CM_NO_TIME_OUT                   (1 << 5)
#define CM_LOW_GRAPHICS                  (1 << 6)
#define CM_AI_EASY                       (1 << 7)
#define CM_AI_MEDIUM                     (1 << 8)
#define CM_AI_HARD                       (1 << 9)

#endif
