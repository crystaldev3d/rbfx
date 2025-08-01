//
// Copyright (c) 2008-2022 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/// \file

#pragma once

#include "Urho3D/Math/BoundingBox.h"
#include "Urho3D/Navigation/NavigationDefs.h"
#include "Urho3D/Scene/Component.h"

#include <EASTL/unique_ptr.h>
#include <EASTL/unordered_set.h>

class dtNavMesh;
class dtNavMeshQuery;
class dtQueryFilter;

namespace Urho3D
{

enum NavmeshPartitionType
{
    NAVMESH_PARTITION_WATERSHED = 0,
    NAVMESH_PARTITION_MONOTONE
};

class Geometry;
class NavArea;
class Navigable;

struct FindPathData;
struct NavBuildData;
struct NavigationGeometryInfo;

/// A flag representing the type of path point- none, the start of a path segment, the end of one, or an off-mesh connection.
enum NavigationPathPointFlag
{
    NAVPATHFLAG_NONE = 0,
    NAVPATHFLAG_START = 0x01,
    NAVPATHFLAG_END = 0x02,
    NAVPATHFLAG_OFF_MESH = 0x04
};

struct URHO3D_API NavigationPathPoint
{
    /// World-space position of the path point.
    Vector3 position_;
    /// Detour flag.
    NavigationPathPointFlag flag_;
    /// Detour area ID.
    unsigned char areaID_;
};

/// Navigation mesh component. Collects the navigation geometry from child nodes with the Navigable component and responds to path queries.
class URHO3D_API NavigationMesh : public Component
{
    URHO3D_OBJECT(NavigationMesh, Component);

    friend class CrowdManager;

public:
    /// Version of compiled navigation data. Navigation data should be discarded and rebuilt on mismatch.
    static constexpr int NavigationDataVersion = 1;
    /// Default maximum number of tiles.
    static constexpr int DefaultMaxTiles = 256;
    /// Maximum number of layers in the single tile.
    static constexpr unsigned MaxLayers = 255;

    /// Construct.
    explicit NavigationMesh(Context* context);
    /// Destruct.
    ~NavigationMesh() override;
    /// Register object factory.
    /// @nobind
    static void RegisterObject(Context* context);

    /// Visualize the component as debug geometry.
    void DrawDebugGeometry(DebugRenderer* debug, bool depthTest) override;

    /// Set maximum number of tiles.
    void SetMaxTiles(int maxTiles) { maxTiles_ = Max(maxTiles, 1); }
    /// Set tile size.
    /// @property
    void SetTileSize(int size);
    /// Set cell size.
    /// @property
    void SetCellSize(float size);
    /// Set cell height.
    /// @property
    void SetCellHeight(float height);
    /// Set min and max height of the navigation mesh, i.e. min and max Y value in world space.
    void SetHeightRange(const Vector2& range) { heightRange_ = range; }
    /// Set navigation agent height.
    /// @property
    void SetAgentHeight(float height);
    /// Set navigation agent radius.
    /// @property
    void SetAgentRadius(float radius);
    /// Set navigation agent max vertical climb.
    /// @property
    void SetAgentMaxClimb(float maxClimb);
    /// Set navigation agent max slope.
    /// @property
    void SetAgentMaxSlope(float maxSlope);
    /// Set region minimum size.
    /// @property
    void SetRegionMinSize(float size);
    /// Set region merge size.
    /// @property
    void SetRegionMergeSize(float size);
    /// Set edge max length.
    /// @property
    void SetEdgeMaxLength(float length);
    /// Set edge max error.
    /// @property
    void SetEdgeMaxError(float error);
    /// Set detail sampling distance.
    /// @property
    void SetDetailSampleDistance(float distance);
    /// Set detail sampling maximum error.
    /// @property
    void SetDetailSampleMaxError(float error);
    /// Set padding of the navigation mesh bounding box. Having enough padding allows to add geometry on the extremities of the navigation mesh when doing partial rebuilds.
    /// @property
    void SetPadding(const Vector3& padding);
    /// Set the cost of an area.
    void SetAreaCost(unsigned areaID, float cost);

    /// Clear navigation mesh data.
    void Clear();
    /// Allocate the navigation mesh without building any tiles. Return true if successful.
    bool Allocate();
    /// Rebuild part of the navigation mesh contained by the world-space bounding box. Return true if successful.
    bool BuildTilesInRegion(const BoundingBox& boundingBox);
    /// Rebuild part of the navigation mesh in the rectangular area. Return true if successful.
    bool BuildTiles(const IntVector2& from, const IntVector2& to);
    /// Rebuild the navigation mesh allocating sufficient maximum number of tiles. Return true if successful.
    bool Rebuild();

    /// Enumerate all tiles.
    ea::vector<IntVector2> GetAllTileIndices() const;
    /// Return tile data.
    virtual ea::vector<unsigned char> GetTileData(const IntVector2& tileIndex) const;
    /// Add tile to navigation mesh.
    virtual bool AddTile(const ea::vector<unsigned char>& tileData);
    /// Remove tile from navigation mesh.
    virtual void RemoveTile(const IntVector2& tileIndex);
    /// Remove all tiles from navigation mesh.
    virtual void RemoveAllTiles();
    /// Return whether the navigation mesh has tile.
    bool HasTile(const IntVector2& tileIndex) const;
    /// Return bounding box of the tile in the world space. Y coordinate spans from -infinity to infinity.
    BoundingBox GetTileBoundingBoxColumn(const IntVector2& tileIndex) const;
    /// Return index of the tile at the position.
    IntVector2 GetTileIndex(const Vector3& position) const;
    /// Find the nearest point on the navigation mesh to a given point. Extents specifies how far out from the specified point to check along each axis.
    Vector3 FindNearestPoint
        (const Vector3& point, const Vector3& extents = Vector3::ONE, const dtQueryFilter* filter = nullptr, dtPolyRef* nearestRef = nullptr);
    /// Try to move along the surface from one point to another.
    Vector3 MoveAlongSurface(const Vector3& start, const Vector3& end, const Vector3& extents = Vector3::ONE, int maxVisited = 3,
        const dtQueryFilter* filter = nullptr);
    /// Find a path between world space points. Return non-empty list of points if successful. Extents specifies how far off the navigation mesh the points can be.
    void FindPath(ea::vector<Vector3>& dest, const Vector3& start, const Vector3& end, const Vector3& extents = Vector3::ONE,
        const dtQueryFilter* filter = nullptr);
    /// Find a path between world space points. Return non-empty list of navigation path points if successful. Extents specifies how far off the navigation mesh the points can be.
    void FindPath
        (ea::vector<NavigationPathPoint>& dest, const Vector3& start, const Vector3& end, const Vector3& extents = Vector3::ONE,
            const dtQueryFilter* filter = nullptr);
    /// Return a random point on the navigation mesh.
    Vector3 GetRandomPoint(const dtQueryFilter* filter = nullptr, dtPolyRef* randomRef = nullptr);
    /// Return a random point on the navigation mesh within a circle. The circle radius is only a guideline and in practice the returned point may be further away.
    Vector3 GetRandomPointInCircle
        (const Vector3& center, float radius, const Vector3& extents = Vector3::ONE, const dtQueryFilter* filter = nullptr,
            dtPolyRef* randomRef = nullptr);
    /// Return distance to wall from a point. Maximum search radius must be specified.
    float GetDistanceToWall
        (const Vector3& point, float radius, const Vector3& extents = Vector3::ONE, const dtQueryFilter* filter = nullptr,
            Vector3* hitPos = nullptr, Vector3* hitNormal = nullptr);
    /// Perform a walkability raycast on the navigation mesh between start and end and return the point where a wall was hit, or the end point if no walls.
    Vector3 Raycast
        (const Vector3& start, const Vector3& end, const Vector3& extents = Vector3::ONE, const dtQueryFilter* filter = nullptr,
            Vector3* hitNormal = nullptr);
    /// Add debug geometry to the debug renderer.
    void DrawDebugGeometry(bool depthTest);

    /// Return the given name of this navigation mesh.
    ea::string GetMeshName() const { return meshName_; }

    /// Set the name of this navigation mesh.
    void SetMeshName(const ea::string& newName);

    /// Return maximum number of tiles.
    int GetMaxTiles() const { return maxTiles_; }

    /// Return tile size.
    /// @property
    int GetTileSize() const { return tileSize_; }

    /// Return cell size.
    /// @property
    float GetCellSize() const { return cellSize_; }

    /// Return cell height.
    /// @property
    float GetCellHeight() const { return cellHeight_; }

    /// Return min and max height of the navigation mesh, i.e. min and max Y value in world space.
    const Vector2& GetHeightRange() const { return heightRange_; }

    /// Return whether the height range is valid.
    bool IsHeightRangeValid() const { return heightRange_.x_ < heightRange_.y_; }

    /// Return navigation agent height.
    /// @property
    float GetAgentHeight() const { return agentHeight_; }

    /// Return navigation agent radius.
    /// @property
    float GetAgentRadius() const { return agentRadius_; }

    /// Return navigation agent max vertical climb.
    /// @property
    float GetAgentMaxClimb() const { return agentMaxClimb_; }

    /// Return navigation agent max slope.
    /// @property
    float GetAgentMaxSlope() const { return agentMaxSlope_; }

    /// Return region minimum size.
    /// @property
    float GetRegionMinSize() const { return regionMinSize_; }

    /// Return region merge size.
    /// @property
    float GetRegionMergeSize() const { return regionMergeSize_; }

    /// Return edge max length.
    /// @property
    float GetEdgeMaxLength() const { return edgeMaxLength_; }

    /// Return edge max error.
    /// @property
    float GetEdgeMaxError() const { return edgeMaxError_; }

    /// Return detail sampling distance.
    /// @property
    float GetDetailSampleDistance() const { return detailSampleDistance_; }

    /// Return detail sampling maximum error.
    /// @property
    float GetDetailSampleMaxError() const { return detailSampleMaxError_; }

    /// Return navigation mesh bounding box padding.
    /// @property
    const Vector3& GetPadding() const { return padding_; }

    /// Get the current cost of an area.
    float GetAreaCost(unsigned areaID) const;

    /// Return whether has been initialized with valid navigation data.
    /// @property
    bool IsInitialized() const { return navMesh_ != nullptr; }

    /// Set the partition type used for polygon generation.
    /// @property
    void SetPartitionType(NavmeshPartitionType partitionType);

    /// Return Partition Type.
    /// @property
    NavmeshPartitionType GetPartitionType() const { return partitionType_; }

    /// Set navigation data attribute.
    virtual void SetNavigationDataAttr(const ea::vector<unsigned char>& value);
    /// Return navigation data attribute.
    virtual ea::vector<unsigned char> GetNavigationDataAttr() const;

    /// Draw debug geometry for OffMeshConnection components.
    /// @property
    void SetDrawOffMeshConnections(bool enable) { drawOffMeshConnections_ = enable; }

    /// Return whether to draw OffMeshConnection components.
    /// @property
    bool GetDrawOffMeshConnections() const { return drawOffMeshConnections_; }

    /// Draw debug geometry for NavArea components.
    /// @property
    void SetDrawNavAreas(bool enable) { drawNavAreas_ = enable; }

    /// Return whether to draw NavArea components.
    /// @property
    bool GetDrawNavAreas() const { return drawNavAreas_; }

private:
    /// Read tile data to the navigation mesh.
    bool ReadTile(Deserializer& source, bool silent);

protected:
    /// Allocate the navigation mesh without building any tiles. Return true if successful.
    virtual bool AllocateMesh(unsigned maxTiles);
    /// Rebuild the navigation mesh allocating sufficient maximum number of tiles. Return true if successful.
    virtual bool RebuildMesh();
    /// Build mesh tiles from the geometry data. Return true if successful.
    virtual unsigned BuildTilesFromGeometry(
        ea::vector<NavigationGeometryInfo>& geometryList, const IntVector2& from, const IntVector2& to);

    /// Send rebuild event.
    void SendRebuildEvent();
    /// Send tile added event.
    void SendTileAddedEvent(const IntVector2& tileIndex);
    /// Collect geometry from under Navigable components.
    void CollectGeometries(ea::vector<NavigationGeometryInfo>& geometryList);
    /// Visit nodes and collect navigable geometry.
    void CollectGeometries(ea::vector<NavigationGeometryInfo>& geometryList, Navigable* navigable, Node* node,
        ea::hash_set<Node*>& processedNodes, bool recursive);
    /// Get geometry data within a bounding box.
    void GetTileGeometry(NavBuildData* build, const ea::vector<NavigationGeometryInfo>& geometryList, BoundingBox& box);
    /// Add a triangle mesh to the geometry data.
    void AddTriMeshGeometry(NavBuildData* build, Geometry* geometry, const Matrix3x4& transform, unsigned char areaId);
    /// Build one tile of the navigation mesh. Return true if successful.
    bool BuildTile(ea::vector<NavigationGeometryInfo>& geometryList, int x, int z);
    /// Ensure that the navigation mesh query is initialized. Return true if successful.
    bool InitializeQuery();
    /// Release the navigation mesh and the query.
    virtual void ReleaseNavigationMesh();

    /// Draw debug geometry for single tile.
    void DrawDebugTileGeometry(DebugRenderer* debug, bool depthTest, int tileIndex);

    /// Identifying name for this navigation mesh.
    ea::string meshName_;
    /// Detour navigation mesh.
    dtNavMesh* navMesh_;
    /// Detour navigation mesh query.
    dtNavMeshQuery* navMeshQuery_;
    /// Detour navigation mesh query filter.
    ea::unique_ptr<dtQueryFilter> queryFilter_;
    /// Temporary data for finding a path.
    ea::unique_ptr<FindPathData> pathData_;
    /// Maximum number of tiles.
    int maxTiles_{DefaultMaxTiles};
    /// Tile size.
    int tileSize_;
    /// Cell size.
    float cellSize_;
    /// Cell height.
    float cellHeight_;
    /// Total height range of the navigation mesh, in world space.
    Vector2 heightRange_;
    /// Navigation agent height.
    float agentHeight_;
    /// Navigation agent radius.
    float agentRadius_;
    /// Navigation agent max vertical climb.
    float agentMaxClimb_;
    /// Navigation agent max slope.
    float agentMaxSlope_;
    /// Region minimum size.
    float regionMinSize_;
    /// Region merge size.
    float regionMergeSize_;
    /// Edge max length.
    float edgeMaxLength_;
    /// Edge max error.
    float edgeMaxError_;
    /// Detail sampling distance.
    float detailSampleDistance_;
    /// Detail sampling maximum error.
    float detailSampleMaxError_;
    /// Bounding box padding.
    Vector3 padding_;
    /// Type of the heightfield partitioning.
    NavmeshPartitionType partitionType_;
    /// Keep internal build resources for debug draw modes.
    bool keepInterResults_;
    /// Debug draw OffMeshConnection components.
    bool drawOffMeshConnections_;
    /// Debug draw NavArea components.
    bool drawNavAreas_;
    /// NavAreas for this NavMesh.
    ea::vector<WeakPtr<NavArea> > areas_;
};

/// Register Navigation library objects.
/// @nobind
void URHO3D_API RegisterNavigationLibrary(Context* context);

}
