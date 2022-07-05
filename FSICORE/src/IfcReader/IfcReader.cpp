#include "fsipch.h"
#define IfcSchema Ifc4

#include <IfcOpenShell/include/ifcparse/IfcFile.h>
#include <IfcOpenShell/include/ifcparse/Ifc4.h>
#include <IfcOpenShell/include/ifcgeom/IfcGeom.h>


#include <IfcOpenShell/include/ifcparse/IfcBaseClass.h>
#include <IfcOpenShell/include/ifcparse/IfcHierarchyHelper.h>

#include <IfcOpenShell/include/ifcgeom_schema_agnostic/IfcGeomFilter.h>
#include <IfcOpenShell/include/ifcgeom_schema_agnostic/IfcGeomIterator.h>
#include <IfcOpenShell/include/ifcgeom_schema_agnostic/IfcGeomRenderStyles.h>

#include "IfcReader.h" 
#include "FSIObjectTypes.h"

namespace fsi {
    ObjectPtr resolveObjects(IfcUtil::IfcBaseEntity *pRootEntity, std::map<std::string, ObjectPtr> &objects) {
        IfcSchema::IfcObjectDefinition *pObjDef = pRootEntity->as<IfcSchema::IfcObjectDefinition>();
        if (!pObjDef) {
            return nullptr;
        }

        ObjectPtr pObject;
        if      (IfcSchema::IfcWall *pIfcWall = pObjDef->as<IfcSchema::IfcWall>()) {
            WallPtr pWall = std::make_shared<Wall>();
            if (pIfcWall->PredefinedType()) {
                pWall->m_type = pIfcWall->PredefinedType().get();
            }
            pObject = pWall;
        }
        else if (IfcSchema::IfcWallStandardCase *pIfcWall = pObjDef->as<IfcSchema::IfcWallStandardCase>()) {
            WallPtr pWall = std::make_shared<Wall>();
            if (pIfcWall->PredefinedType()) {
                pWall->m_type = pIfcWall->PredefinedType().get();
            }
            pObject = pWall;
        }
        else if (IfcSchema::IfcSlab *pIfcSlab = pObjDef->as<IfcSchema::IfcSlab>()) {
            SlabPtr pSlab = std::make_shared<Slab>();
            if (pIfcSlab->PredefinedType()) {
                pSlab->m_type = pIfcSlab->PredefinedType().get();
            }
            pObject = pSlab;
        }
        else if (IfcSchema::IfcColumn *pIfcColumn = pObjDef->as<IfcSchema::IfcColumn>()) {
            ColumnPtr pColumn = std::make_shared<Column>();
            if (pIfcColumn->PredefinedType()) {
                pColumn->m_type = pIfcColumn->PredefinedType().get();
            }
            pObject = pColumn;
        }
        else if (IfcSchema::IfcBeam *pIfcBeam = pObjDef->as<IfcSchema::IfcBeam>()) {
            BeamPtr pBeam = std::make_shared<Beam>();
            if (pIfcBeam->PredefinedType()) {
                pBeam->m_type = pIfcBeam->PredefinedType().get();
            }
            pObject = pBeam;
        }
        else if (IfcSchema::IfcMember *pIfcMember = pObjDef->as<IfcSchema::IfcMember>()) {
            MemberPtr pMember = std::make_shared<Member>();
            if (pIfcMember->PredefinedType()) {
                pMember->m_type = pIfcMember->PredefinedType().get();
            }
            pObject = pMember;
        }
        else if (IfcSchema::IfcRailing *pIfcRailing = pObjDef->as<IfcSchema::IfcRailing>()) {
            RailingPtr pRailing = std::make_shared<Railing>();
            if (pIfcRailing->PredefinedType()) {
                pRailing->m_type = pIfcRailing->PredefinedType().get();
            }
            pObject = pRailing;
        }
        else if (IfcSchema::IfcRoof *pIfcRoof = pObjDef->as<IfcSchema::IfcRoof>()) {
            RoofPtr pRoof = std::make_shared<Roof>();
            if (pIfcRoof->PredefinedType()) {
                pRoof->m_type = pIfcRoof->PredefinedType().get();
            }
            pObject = pRoof;
        }
        else if (IfcSchema::IfcStair *pIfcStair = pObjDef->as<IfcSchema::IfcStair>()) {
            StairPtr pStair = std::make_shared<Stair>();
            if (pIfcStair->PredefinedType()) {
                pStair->m_type = pIfcStair->PredefinedType().get();
            }
            pObject = pStair;
        }
        else if (IfcSchema::IfcStairFlight *pIfcStairFlight = pObjDef->as<IfcSchema::IfcStairFlight>()) {
            StairFlightPtr pStairFlight = std::make_shared<StairFlight>();
            if (pIfcStairFlight->PredefinedType()) {
                pStairFlight->m_type = pIfcStairFlight->PredefinedType().get();
            }
            pObject = pStairFlight;
        }
        else if (IfcSchema::IfcDoor *pIfcDoor = pObjDef->as<IfcSchema::IfcDoor>()) {
            DoorPtr pDoor = std::make_shared<Door>();
            if (pIfcDoor->PredefinedType()) {
                pDoor->m_type = pIfcDoor->PredefinedType().get();
            }
            pObject = pDoor;
        }
        else if (IfcSchema::IfcWindow *pIfcWindow = pObjDef->as<IfcSchema::IfcWindow>()) {
            WindowPtr pWindow = std::make_shared<Window>();
            if (pIfcWindow->PredefinedType()) {
                pWindow->m_type = pIfcWindow->PredefinedType().get();
            }
            pObject = pWindow;
        }
        else if (IfcSchema::IfcSpace *pIfcSpace = pObjDef->as<IfcSchema::IfcSpace>()) {
            SpacePtr pSpace = std::make_shared<Space>();
            if (pIfcSpace->PredefinedType()) {
                pSpace->m_type = pIfcSpace->PredefinedType().get();
            }
            pObject = pSpace;
        }
        else if (IfcSchema::IfcBuilding *pIfcBuilding = pObjDef->as<IfcSchema::IfcBuilding>()) {
            BuildingPtr pBuilding = std::make_shared<Building>();
            if (pIfcBuilding->CompositionType()) {
                pBuilding->m_type = pIfcBuilding->CompositionType().get();
            }
            pObject = pBuilding;
        }
        else if (IfcSchema::IfcBuildingStorey *pIfcBuildingStorey = pObjDef->as<IfcSchema::IfcBuildingStorey>()) {
            BuildingStoreyPtr pBuildingStorey = std::make_shared<BuildingStorey>();
            if (pIfcBuildingStorey->CompositionType()) {
                pBuildingStorey->m_type = pIfcBuildingStorey->CompositionType().get();
            }
            pObject = pBuildingStorey;
        }
        else if (IfcSchema::IfcBuildingElement *pIfcBuildingEl = pObjDef->as<IfcSchema::IfcBuildingElement>()) {
            pObject = std::make_shared<GeometryObject>();
        }
        else {
            pObject = std::make_shared<Object>();
        }

        // common atributes
        pObject->m_id = pObjDef->identity();
        pObject->m_guid = pObjDef->GlobalId();
        if (pObjDef->Name()) {
            pObject->m_name = pObjDef->Name().get();
        }
        if (pObjDef->Description()) {
            pObject->m_description = pObjDef->Description().get();
        }

        auto pRelAggregatesList = pObjDef->IsDecomposedBy();
        if (pRelAggregatesList) {
            for (auto pRelAggregates : *pRelAggregatesList) {
                auto pRelatedObjectsList = pRelAggregates->RelatedObjects();
                if (pRelatedObjectsList) {
                    for (auto pRelatedObject : *pRelatedObjectsList) {
                        ObjectPtr pChildObject = resolveObjects(pRelatedObject, objects);
                        if (pChildObject) {
                            pObject->m_children.push_back(pChildObject);
                            pChildObject->m_parent = pObject;
                        }
                    }
                }
            }
        }
        IfcSchema::IfcSpatialStructureElement *pSpatialStructureEl = pObjDef->as<IfcSchema::IfcSpatialStructureElement>();
        if (pSpatialStructureEl) {
            auto pRelContainedList = pSpatialStructureEl->ContainsElements();
            if (pRelContainedList) {
                for (auto pRelContained : *pRelContainedList) {
                    if (pRelContained) {
                        auto pRelatedElementList = pRelContained->RelatedElements();
                        if (pRelatedElementList) {
                            for (auto pRelatedElement : *pRelatedElementList) {
                                ObjectPtr pChildObject = resolveObjects(pRelatedElement, objects);
                                if (pChildObject) {
                                    pObject->m_children.push_back(pChildObject);
                                    pChildObject->m_parent = pObject;
                                }
                            }
                        }
                    }
                }
            }
        }
        objects[pObject->m_guid] = pObject;
        return pObject;
    }

    FSIScenePtr IfcReader::read(const std::string &filename) {
        // read ifc file
        std::shared_ptr<IfcParse::IfcFile> pFile = std::make_shared<IfcParse::IfcFile>(filename);
        if (!pFile->good()) {
            return nullptr;
        }

        // get IfcProject object
        IfcSchema::IfcProject::list::ptr elements = pFile->instances_by_type<IfcSchema::IfcProject>();
        if (!elements->size()) {
            return nullptr;
        }

        // resolve hierarchy
        std::map<std::string, ObjectPtr> objects;
        ObjectPtr pRootObject = resolveObjects(*elements->begin(), objects);
        if (!pRootObject || objects.empty()) {
            return nullptr;
        }

        // convert geometry
        IfcGeom::IteratorSettings settings;
        settings.set(IfcGeom::IteratorSettings::APPLY_DEFAULT_MATERIALS,      true);
        settings.set(IfcGeom::IteratorSettings::USE_WORLD_COORDS,             true);
        settings.set(IfcGeom::IteratorSettings::WELD_VERTICES,                false);
        settings.set(IfcGeom::IteratorSettings::SEW_SHELLS,                   false); // svg depends on correct solids for boolean subtractions for hlr
        settings.set(IfcGeom::IteratorSettings::DISABLE_TRIANGULATION,        true);
        settings.set(IfcGeom::IteratorSettings::ELEMENT_HIERARCHY,        false); // svg depends on correct solids for boolean subtractions for hlr
        settings.set(IfcGeom::IteratorSettings::CONVERT_BACK_UNITS, false);

        // create geometry iterator
        IfcGeom::Iterator context_iterator(settings, pFile.get(), 4);
        context_iterator.initialize();
        do {
            IfcGeom::BRepElement* geom_object = context_iterator.get_native();
            if (objects.find(geom_object->guid()) != objects.end()) {
                ObjectPtr pObject = objects[geom_object->guid()];
                // if object is geometry object
                if (GeometryObjectPtr pGeomObject = std::dynamic_pointer_cast<GeometryObject>(pObject)) {
                    pGeomObject->m_shape = std::make_shared<TopoDS_Shape>(geom_object->geometry().as_compound());
                    continue;
                }
                // if type of the object was not recognized make him GeometryObject
                GeometryObjectPtr pGeomObject = std::make_shared<GeometryObject>();
                pGeomObject->m_id = pObject->m_id;
                pGeomObject->m_name = pObject->m_name;
                pGeomObject->m_description = pObject->m_description;
                pGeomObject->m_guid = pObject->m_guid;
                pGeomObject->m_parent = pObject->m_parent;
                pGeomObject->m_children = pObject->m_children;
                pGeomObject->m_shape = std::make_shared<TopoDS_Shape>(geom_object->geometry().as_compound());
                objects[geom_object->guid()] = pGeomObject;
            }
        } 
        while (context_iterator.next());

        FSIScenePtr pScene = std::make_shared<FSIScene>();
        for (auto &pair : objects) {
            pScene->m_objects.push_back(pair.second);
            pScene->m_root = pRootObject;
        }

        return pScene;
    }
}

