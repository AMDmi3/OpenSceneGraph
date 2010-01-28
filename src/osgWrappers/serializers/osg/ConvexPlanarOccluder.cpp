#include <osg/ConvexPlanarOccluder>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

static void readConvexPlanarPolygon( osgDB::InputStream& is, osg::ConvexPlanarPolygon& polygon )
{
    unsigned int size = 0; is >> size >> osgDB::BEGIN_BRACKET;
    for ( unsigned int i=0; i<size; ++i )
    {
        osg::Vec3d vertex; is >> vertex;
        polygon.add( vertex );
    }
    is >> osgDB::END_BRACKET;
}

static void writeConvexPlanarPolygon( osgDB::OutputStream& os, const osg::ConvexPlanarPolygon& polygon )
{
    const osg::ConvexPlanarPolygon::VertexList& vertices = polygon.getVertexList();
    os << vertices.size() << osgDB::BEGIN_BRACKET << std::endl;
    for ( osg::ConvexPlanarPolygon::VertexList::const_iterator itr=vertices.begin();
          itr!=vertices.end(); ++itr )
    {
        os << osg::Vec3d(*itr) << std::endl;
    }
    os << osgDB::END_BRACKET << std::endl;
}

// _occluder
static bool checkOccluder( const osg::ConvexPlanarOccluder& obj )
{
    return obj.getOccluder().getVertexList().size()>0;
}

static bool readOccluder( osgDB::InputStream& is, osg::ConvexPlanarOccluder& obj )
{
    osg::ConvexPlanarPolygon polygon;
    readConvexPlanarPolygon( is, polygon );
    obj.setOccluder( polygon );
    return true;
}

static bool writeOccluder( osgDB::OutputStream& os, const osg::ConvexPlanarOccluder& obj )
{
    writeConvexPlanarPolygon( os, obj.getOccluder() );
    return true;
}

// _holeList
static bool checkHoles( const osg::ConvexPlanarOccluder& obj )
{
    return obj.getHoleList().size()>0;
}

static bool readHoles( osgDB::InputStream& is, osg::ConvexPlanarOccluder& obj )
{
    unsigned int size = 0; is >> size >> osgDB::BEGIN_BRACKET;
    for ( unsigned int i=0; i<size; ++i )
    {
        osg::ConvexPlanarPolygon polygon;
        is >> osgDB::PROPERTY("Polygon");
        readConvexPlanarPolygon( is, polygon );
        obj.addHole( polygon );
    }
    is >> osgDB::END_BRACKET;
    return true;
}

static bool writeHoles( osgDB::OutputStream& os, const osg::ConvexPlanarOccluder& obj )
{
    const osg::ConvexPlanarOccluder::HoleList& holes = obj.getHoleList();
    os << holes.size() << osgDB::BEGIN_BRACKET << std::endl;
    for ( osg::ConvexPlanarOccluder::HoleList::const_iterator itr=holes.begin();
          itr!=holes.end(); ++itr )
    {
        os << osgDB::PROPERTY("Polygon");
        writeConvexPlanarPolygon( os, *itr );
    }
    os << osgDB::END_BRACKET << std::endl;
    return true;
}

REGISTER_OBJECT_WRAPPER( ConvexPlanarOccluder,
                         new osg::ConvexPlanarOccluder,
                         osg::ConvexPlanarOccluder,
                         "osg::Object osg::ConvexPlanarOccluder" )
{
    ADD_USER_SERIALIZER( Occluder );  // _occluder
    ADD_USER_SERIALIZER( Holes );  // _holeList
}
