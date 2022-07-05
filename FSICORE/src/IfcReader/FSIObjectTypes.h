
#pragma once

#include "FSIBasicTypes.h"

namespace fsi {
	// building object
	class Building : virtual public ObjectType, public Object {
	public:
		Building() = default;
        virtual ~Building() = default;

		inline virtual const char* className() const {
			return "Building";
		}
	};
	typedef std::shared_ptr<Building> BuildingPtr;

	// building storey object
	class BuildingStorey : virtual public ObjectType, public Object {
	public:
		BuildingStorey() = default;
        virtual ~BuildingStorey() = default;

		inline virtual const char* className() const {
			return "BuildingStorey";
		}
	};
	typedef std::shared_ptr<BuildingStorey> BuildingStoreyPtr;

    // wall object
	class Wall : virtual public ObjectType, public GeometryObject {
	public:
		Wall() = default;
        virtual ~Wall() = default;

		inline virtual const char* className() const {
			return "Wall";
		}
	};
	typedef std::shared_ptr<Wall> WallPtr;

	// slab object
	class Slab : virtual public ObjectType, public GeometryObject {
	public:
		Slab() = default;
        virtual ~Slab() = default;

		inline virtual const char* className() const {
			return "Slab";
		}
	};
	typedef std::shared_ptr<Slab> SlabPtr;

	// column object
	class Column : virtual public ObjectType, public GeometryObject {
	public:
		Column() = default;
        virtual ~Column() = default;

		inline virtual const char* className() const {
			return "Column";
		}
	};
	typedef std::shared_ptr<Column> ColumnPtr;

	// beam object
	class Beam : virtual public ObjectType, public GeometryObject {
	public:
		Beam() = default;
        virtual ~Beam() = default;

		inline virtual const char* className() const {
			return "Beam";
		}
	};
	typedef std::shared_ptr<Beam> BeamPtr;

	// member object
	class Member : virtual public ObjectType, public GeometryObject {
	public:
		Member() = default;
        virtual ~Member() = default;

		inline virtual const char* className() const {
			return "Member";
		}
	};
	typedef std::shared_ptr<Member> MemberPtr;

	// railing object
	class Railing : virtual public ObjectType, public GeometryObject {
	public:
		Railing() = default;
        virtual ~Railing() = default;

		inline virtual const char* className() const {
			return "Railing";
		}
	};
	typedef std::shared_ptr<Railing> RailingPtr;

	// roof object
	class Roof : virtual public ObjectType, public GeometryObject {
	public:
		Roof() = default;
        virtual ~Roof() = default;

		inline virtual const char* className() const {
			return "Roof";
		}
	};
	typedef std::shared_ptr<Roof> RoofPtr;

	// stair object
	class Stair : virtual public ObjectType, public GeometryObject {
	public:
		Stair() = default;
        virtual ~Stair() = default;

		inline virtual const char* className() const {
			return "Stair";
		}
	};
	typedef std::shared_ptr<Stair> StairPtr;

	// stair flight object
	class StairFlight : virtual public ObjectType, public GeometryObject {
	public:
		StairFlight() = default;
        virtual ~StairFlight() = default;

		inline virtual const char* className() const {
			return "StairFlight";
		}
	};
	typedef std::shared_ptr<StairFlight> StairFlightPtr;
	
	// door object
	class Door : virtual public ObjectType, ObjectOperationType, public GeometryObject {
	public:
		Door() = default;
        virtual ~Door() = default;

		inline virtual const char* className() const {
			return "Door";
		}
	};
	typedef std::shared_ptr<Door> DoorPtr;

	// window object
	class Window : virtual public ObjectType, ObjectPartitioningType, public GeometryObject {
	public:
		Window() = default;
        virtual ~Window() = default;

		inline virtual const char* className() const {
			return "Window";
		}
	};
	typedef std::shared_ptr<Window> WindowPtr;

	// space object
	class Space : virtual public ObjectType, public GeometryObject {
	public:
		Space() = default;
        virtual ~Space() = default;

		inline virtual const char* className() const {
			return "Space";
		}

		std::wstring m_longName;
	};
	typedef std::shared_ptr<Space> SpacePtr;

	// sprinkler
	class Sprinkler : public GeometryObject {
	public:
		Sprinkler() = default;
		virtual ~Sprinkler() = default;

		inline virtual const char* className() const {
			return "Sprinkler";
		}

		glm::vec3 m_position;
		float m_diameter;
	};
	typedef std::shared_ptr<Sprinkler> SprinklerPtr;
}

