// tilemgr.hxx -- routines to handle dynamic management of scenery tiles
//
// Written by Curtis Olson, started January 1998.
//
// Copyright (C) 1997  Curtis L. Olson  - http://www.flightgear.org/~curt
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id$


#ifndef _TILEMGR_HXX
#define _TILEMGR_HXX

#include <simgear/compiler.h>

#include <simgear/structure/subsystem_mgr.hxx>
#include <simgear/bucket/newbucket.hxx>
#include <simgear/scene/tgdb/TileEntry.hxx>
#include <simgear/scene/tgdb/TileCache.hxx>

class SGReaderWriterBTGOptions;

namespace osg
{
class Node;
}

namespace simgear
{
class SGTerraSync;
}

class FGTileMgr : public SGSubsystem, public simgear::ModelLoadHelper {

private:

    // Tile loading state
    enum load_state {
	Start = 0,
	Inited = 1,
	Running = 2
    };

    load_state state;
    
    // schedule a tile for loading, returns true when tile is already loaded
    bool sched_tile( const SGBucket& b, double priority,bool current_view, double request_time);

    // schedule a needed buckets for loading
    void schedule_needed(const SGBucket& curr_bucket, double rangeM);

    SGBucket previous_bucket;
    SGBucket current_bucket;
    SGBucket pending;
    osg::ref_ptr<SGReaderWriterBTGOptions> _options;

    // x and y distance of tiles to load/draw
    float vis;
    int xrange, yrange;

    // current longitude latitude
    double longitude;
    double latitude;
    double scheduled_visibility;

    /**
     * tile cache
     */
    simgear::TileCache tile_cache;
    simgear::SGTerraSync* _terra_sync;

    // Update the various queues maintained by the tilemagr (private
    // internal function, do not call directly.)
    void update_queues();
    
    SGPropertyNode* _visibilityMeters;
    SGPropertyChangeListener* _propListener;
    SGPropertyNode_ptr _randomObjects;
    SGPropertyNode_ptr _randomVegetation;
    SGPropertyNode_ptr _maxTileRangeM;
    
public:
    FGTileMgr();

    ~FGTileMgr();

    // Initialize the Tile Manager
    virtual void init();
    virtual void reinit();

    virtual void update(double dt);

    // update loader configuration options
    void configChanged();

    int schedule_tiles_at(const SGGeod& location, double rangeM);


    const SGBucket& get_current_bucket () const { return current_bucket; }

    /// Returns true if scenery is available for the given lat, lon position
    /// within a range of range_m.
    /// lat and lon are expected to be in degrees.
    bool schedule_scenery(const SGGeod& position, double range_m, double duration=0.0);

    // Load a model for a tile
    osg::Node* loadTileModel(const string& modelPath, bool cacheModel);

    // Returns true if tiles around current view position have been loaded
    bool isSceneryLoaded();
};


#endif // _TILEMGR_HXX