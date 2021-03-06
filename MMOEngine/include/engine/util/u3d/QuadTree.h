/*
Copyright (C) 2007 <SWGEmu>. All rights reserved.
Distribution of this file for usage outside of Core3 is prohibited.
*/

#ifndef QUADTREE_H_
#define QUADTREE_H_

/*
   Quad tree interface
   Copyright (C) 2003 by The Arianne Team
   Copyright (C) 2005 Team OpenWoW
   Copyright (C) 2006 Team SWGEmu

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "system/lang.h"

#include "engine/log/Logger.h"

#ifdef WITH_STM
#include "engine/stm/TransactionalReference.h"
#endif

/**
 * A quad tree is a 2D data structure that keeps lots of objects nicely
 * sorted in a tree with four children in every node, so that queries like
 * "give me a list of objects that are near the point X,Y" works lighting
 * fast.
 */

#include "QuadTreeEntry.h"

#include "QuadTreeNode.h"

namespace engine {
  namespace util {
  	namespace u3d {

	class QuadTree : public Object {
	//#ifdef WITH_STM
		TransactionalReference<QuadTreeNode*> root;
	/*#else
		Reference<QuadTreeNode*> root;
	#endif*/

		static bool logTree;

		ReadWriteLock mutex;

	public:
		QuadTree();
		QuadTree(float minx, float miny, float maxx, float maxy);

		~QuadTree();

		Object* clone();
		Object* clone(void* object);

		void free() {
			TransactionalMemoryManager::instance()->destroy(this);
		}

		/**
		 * Clear all the objects from the quadtree and set it to have
		 * given size.
		 */
		void setSize(float minx, float miny, float maxx, float maxy);

		/**
		 * Insert a object into the quad tree.
		 */
		void insert(QuadTreeEntry *obj);

	 	/**
		 * Remove the object from the quad tree.
		 */
		void remove(QuadTreeEntry *obj);

		/**
		 * Remove all objects from the quad tree
		 */
		void removeAll();

		/**
		 * Return a list of all objects found in the given range from the
		 * given point matching a specific type.
		 * @arg x
		 *   The X coordinate
		 * @arg y
		 *   The Y coordinate
		 * @arg range
		 *   The range to search for objects from the given point
		 * @arg Results
		 *   The output vector where all objects in this range will be placed.
		 * @arg TypeMask
		 *   The mask to AND with the Object->GetType (). If you pass 0 here,
		 *   and TypeValue is also 0, the function will return ALL objects.
		 * @arg TypeValue
		 *   This is the value that will be compared with the result of
		 *   (Object->GetType () & TypeMask). If they are equal, the object
		 *   will be considered matching.
		 */
		void inRange(QuadTreeEntry *obj, float range);

		/**
		 * Same as above, but does rlocks the tree and then unlocks when calling addInRangeObject/remove on the entry
		 */
		void safeInRange(QuadTreeEntry* obj, float range);


		/**
		 * Searches for entries that contain x, y point
		 */
		int inRange(float x, float y, SortedVector<ManagedReference<QuadTreeEntry*> >& objects);

		int inRange(float x, float y, float range, SortedVector<ManagedReference<QuadTreeEntry*> >& objects);

	 	/**
		 * Update object's position in the quad tree.
	 	 * Must be called after every time object changes position.
		 * It is cheap to call if the object haven't moved to a different node,
		 * so you should'nt make any unneccessary optimizations to call Update
		 * less often if the object position has't changed.
		 * @arg data
		 *   The object that possibly has changed his position.
		 * @return
		 *   true if object is still within the boundaries of the quad tree
		 *   false if object has moved outside the topmost square.
		 */
		bool update(QuadTreeEntry *obj);

	private:
		void _insert(TransactionalReference<QuadTreeNode*>& node, QuadTreeEntry *obj);
		bool _update(TransactionalReference<QuadTreeNode*>& node, QuadTreeEntry *obj);

		void _inRange(TransactionalReference<QuadTreeNode*>& node, QuadTreeEntry *obj, float range);
		int _inRange(TransactionalReference<QuadTreeNode*>& node, float x, float y, float range, SortedVector<ManagedReference<QuadTreeEntry*> >& objects);
		int _inRange(TransactionalReference<QuadTreeNode*>& node, float x, float y, SortedVector<ManagedReference<QuadTreeEntry*> >& objects);

		void copyObjects(TransactionalReference<QuadTreeNode*> node, float x, float y, float range, SortedVector<ManagedReference<engine::util::u3d::QuadTreeEntry*> >& objects);

	public:
		static void setLogging(bool doLog) {
			logTree = doLog;
		}

		inline static bool doLog() {
			return logTree;
		}
	};

  	} // u3d
  } // namespace util
} // namespace engine

using namespace engine::util::u3d;

#endif /*QUADTREE_H_*/
