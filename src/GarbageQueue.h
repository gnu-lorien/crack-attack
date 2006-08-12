/*
 * GarbageQueue.h
 * 
 * Crack Attack! is the legal property of its developers, whose names 
 * are too numerous to list here.  Please refer to the COPYRIGHT file
 * distributed with this source distribution for a full listing.
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
 */
#ifndef _GARBAGE_QUEUE_H
#define _GARBAGE_QUEUE_H

#include "GarbageQueueElement.h"

#include <vector>


class GarbageQueue {
public:
  GarbageQueue();
  ~GarbageQueue();

  void add ( int height, int width, int flavor );
  void add ( GarbageQueueElement &element );

  int height ( );
  int specialHeight ( );

  void reset ( );

  int removeWithSpecials ( );

  void sendToGenerator ( );

private:

  int cached_height;
  std::vector<GarbageQueueElement> garbage_queue;

  int removeToFirst ( int flavor );
};

#endif
