#pragma once

#include <string>
#include <vector>
#include <memory>

#include <TopoDS_Shape.hxx>

namespace fsi {
	// base class for all fsi objects
	class Object {
	public:
		Object() = default;
		virtual ~Object() = default;
		inline virtual const char* className() const {
			return "Object";
		};

		int m_id;
		std::string m_name;
		std::string m_description;
		std::string m_guid;

		std::shared_ptr<Object> m_parent;
		std::vector<std::shared_ptr<Object>> m_children;
	};
	typedef std::shared_ptr<Object> ObjectPtr;

	// geometry object
	class GeometryObject : public Object {
	public:
		GeometryObject() = default;
		virtual ~GeometryObject() = default;
		inline virtual const char* className() const {
			return "GeometryObject";
		};

		std::shared_ptr<TopoDS_Shape> m_shape;
	};
	typedef std::shared_ptr<GeometryObject> GeometryObjectPtr;

	// object type
	class ObjectType {
	protected:
		ObjectType() = default;
		virtual ~ObjectType() = default;

		inline virtual const char* className() const {
			return "ObjectType";
		};

	public:
		std::string m_type;
	};
	typedef std::shared_ptr<ObjectType> ObjectTypePtr;

	// object operation type
	class ObjectOperationType {
	protected:
		ObjectOperationType() = default;
		virtual ~ObjectOperationType() = default;

	public:
		std::string m_operationType;
		std::string m_userDefOperationType;
	};
	typedef std::shared_ptr<ObjectOperationType> ObjectOperationTypePtr;

	// object partitiong type
	class ObjectPartitioningType {
	protected:
		ObjectPartitioningType() = default;
		virtual ~ObjectPartitioningType() = default;

	public:
		std::string m_partitioningType;
		std::string m_userDefPartitioningType;
	};
	typedef std::shared_ptr<ObjectPartitioningType> ObjectPartitioningTypePtr;
}

