// Copyright 2017 Benjamin Glatzel
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

namespace Intrinsic
{
namespace Core
{
namespace Components
{
// Enums/Flags
namespace SpecularProbeFlags
{
enum Flags
{
  kParallaxCorrected = 0x01
};
}

// Typedefs
typedef Dod::Ref SpecularProbeRef;
typedef _INTR_ARRAY(SpecularProbeRef) SpecularProbeRefArray;

struct SpecularProbeData : Dod::Components::ComponentDataBase
{
  SpecularProbeData()
      : Dod::Components::ComponentDataBase(
            _INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT)
  {
    descRadius.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descPriority.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descFalloffRangePerc.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descFalloffExponent.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descMinExtent.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descMaxExtent.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descFlags.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
    descSpecularTextureNames.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);

    flags.resize(_INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT);
  }

  _INTR_ARRAY(float) descRadius;
  _INTR_ARRAY(float) descFalloffRangePerc;
  _INTR_ARRAY(float) descFalloffExponent;
  _INTR_ARRAY(glm::vec3) descMinExtent;
  _INTR_ARRAY(glm::vec3) descMaxExtent;
  _INTR_ARRAY(_INTR_ARRAY(Name)) descFlags;
  _INTR_ARRAY(uint32_t) descPriority;
  _INTR_ARRAY(_INTR_ARRAY(Name)) descSpecularTextureNames;

  _INTR_ARRAY(uint32_t) flags;
};

struct SpecularProbeManager
    : Dod::Components::ComponentManagerBase<
          SpecularProbeData, _INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT>
{
  static void init();

  // <-

  _INTR_INLINE static SpecularProbeRef
  createSpecularProbe(Entity::EntityRef p_ParentEntity)
  {
    SpecularProbeRef ref = Dod::Components::ComponentManagerBase<
        SpecularProbeData, _INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT>::
        _createComponent(p_ParentEntity);
    return ref;
  }

  // <-

  _INTR_INLINE static void resetToDefault(MeshRef p_Ref)
  {
    _descRadius(p_Ref) = 20.0f;
    _descFalloffRangePerc(p_Ref) = 0.2f;
    _descFalloffExp(p_Ref) = 1.0f;
    _descMinExtent(p_Ref) = glm::vec3(10.0f, 10.0f, 10.0f);
    _descMaxExtent(p_Ref) = glm::vec3(-10.0f, 0.0f, -10.0f);
    _descFlags(p_Ref).clear();
  }

  // <-

  _INTR_INLINE static void
  destroySpecularProbe(SpecularProbeRef p_SpecularProbe)
  {
    Dod::Components::ComponentManagerBase<
        SpecularProbeData, _INTR_MAX_IRRADIANCE_PROBE_COMPONENT_COUNT>::
        _destroyComponent(p_SpecularProbe);
  }

  // <-

  _INTR_INLINE static void compileDescriptor(SpecularProbeRef p_Ref,
                                             bool p_GenerateDesc,
                                             rapidjson::Value& p_Properties,
                                             rapidjson::Document& p_Document)
  {
    p_Properties.AddMember(
        "radius",
        _INTR_CREATE_PROP_MIN_MAX(p_Document, p_GenerateDesc, _N(SpecularProbe),
                                  _N(float), _descRadius(p_Ref), false, false,
                                  1.0f, 500.0f),
        p_Document.GetAllocator());
    p_Properties.AddMember(
        "falloffRangePerc",
        _INTR_CREATE_PROP_MIN_MAX(p_Document, p_GenerateDesc, _N(SpecularProbe),
                                  _N(float), _descFalloffRangePerc(p_Ref),
                                  false, false, 0.01f, 1.0f),
        p_Document.GetAllocator());
    p_Properties.AddMember(
        "falloffExp",
        _INTR_CREATE_PROP_MIN_MAX(p_Document, p_GenerateDesc, _N(SpecularProbe),
                                  _N(float), _descFalloffExp(p_Ref), false,
                                  false, 0.1f, 10.0f),
        p_Document.GetAllocator());
    p_Properties.AddMember(
        "priority",
        _INTR_CREATE_PROP(p_Document, p_GenerateDesc, _N(SpecularProbe),
                          _N(uint), _descPriority(p_Ref), false, false),
        p_Document.GetAllocator());

    p_Properties.AddMember(
        "flags",
        _INTR_CREATE_PROP_FLAGS(p_Document, p_GenerateDesc, _N(SpecularProbe),
                                "flags", _descFlags(p_Ref), "ParallaxCorrected",
                                false, false),
        p_Document.GetAllocator());

    p_Properties.AddMember(
        "minExtent",
        _INTR_CREATE_PROP(p_Document, p_GenerateDesc, _N(SpecularProbe),
                          _N(vec3), _descMinExtent(p_Ref), false, false),
        p_Document.GetAllocator());
    p_Properties.AddMember(
        "maxExtent",
        _INTR_CREATE_PROP(p_Document, p_GenerateDesc, _N(SpecularProbe),
                          _N(vec3), _descMaxExtent(p_Ref), false, false),
        p_Document.GetAllocator());

    if (!p_GenerateDesc)
    {
      rapidjson::Value specularTextureNames =
          rapidjson::Value(rapidjson::kArrayType);
      for (uint32_t i = 0u; i < _descSpecularTextureNames(p_Ref).size(); ++i)
      {
        specularTextureNames.PushBack(
            _INTR_CREATE_PROP(p_Document, p_GenerateDesc, _N(SpecularProbe),
                              _N(string), _descSpecularTextureNames(p_Ref)[i],
                              false, false),
            p_Document.GetAllocator());
      }
      p_Properties.AddMember("specularTextureNames", specularTextureNames,
                             p_Document.GetAllocator());
    }
  }

  // <-

  _INTR_INLINE static void initFromDescriptor(SpecularProbeRef p_Ref,
                                              bool p_GenerateDesc,
                                              rapidjson::Value& p_Properties)
  {
    if (p_Properties.HasMember("radius"))
      _descRadius(p_Ref) =
          JsonHelper::readPropertyFloat(p_Properties["radius"]);
    if (p_Properties.HasMember("falloffRangePerc"))
      _descFalloffRangePerc(p_Ref) =
          JsonHelper::readPropertyFloat(p_Properties["falloffRangePerc"]);
    if (p_Properties.HasMember("falloffExp"))
      _descFalloffExp(p_Ref) =
          JsonHelper::readPropertyFloat(p_Properties["falloffExp"]);
    if (p_Properties.HasMember("priority"))
      _descPriority(p_Ref) =
          JsonHelper::readPropertyUint(p_Properties["priority"]);
    if (p_Properties.HasMember("minExtent"))
      _descMinExtent(p_Ref) =
          JsonHelper::readPropertyVec3(p_Properties["minExtent"]);
    if (p_Properties.HasMember("maxExtent"))
      _descMaxExtent(p_Ref) =
          JsonHelper::readPropertyVec3(p_Properties["maxExtent"]);

    if (p_Properties.HasMember("flags"))
    {
      _descFlags(p_Ref).clear();
      JsonHelper::readPropertyFlagsNameArray(p_Properties["flags"],
                                             _descFlags(p_Ref));
    }

    if (!p_GenerateDesc)
    {
      if (p_Properties.HasMember("specularTextureNames"))
      {
        _descSpecularTextureNames(p_Ref).clear();
        const rapidjson::Value& specularTextureNames =
            p_Properties["specularTextureNames"];
        for (uint32_t i = 0u; i < specularTextureNames.Size(); ++i)
        {
          _descSpecularTextureNames(p_Ref).push_back(
              JsonHelper::readPropertyName(specularTextureNames[i]));
        }
      }
    }
  }

  // <-

  static void createResources(const SpecularProbeRefArray& p_Probes);
  static void destroyResources(const SpecularProbeRefArray& p_Probes);

  // <-

  _INTR_INLINE static void sortByPriority(SpecularProbeRefArray& p_Probes)
  {
    _INTR_PROFILE_CPU("General", "Sort Specular Probes");

    struct Comparator
    {
      bool operator()(const Dod::Ref& a, const Dod::Ref& b) const
      {
        return _descPriority(a) < _descPriority(b);
      }
    } comp;

    Algorithm::parallelSort<Dod::Ref, Comparator>(p_Probes, comp);
  }

  // <-

  // Description
  _INTR_INLINE static float& _descRadius(SpecularProbeRef p_Ref)
  {
    return _data.descRadius[p_Ref._id];
  }
  _INTR_INLINE static uint32_t& _descPriority(SpecularProbeRef p_Ref)
  {
    return _data.descPriority[p_Ref._id];
  }
  _INTR_INLINE static float& _descFalloffRangePerc(SpecularProbeRef p_Ref)
  {
    return _data.descFalloffRangePerc[p_Ref._id];
  }
  _INTR_INLINE static float& _descFalloffExp(SpecularProbeRef p_Ref)
  {
    return _data.descFalloffExponent[p_Ref._id];
  }
  _INTR_INLINE static glm::vec3& _descMinExtent(SpecularProbeRef p_Ref)
  {
    return _data.descMinExtent[p_Ref._id];
  }
  _INTR_INLINE static glm::vec3& _descMaxExtent(SpecularProbeRef p_Ref)
  {
    return _data.descMaxExtent[p_Ref._id];
  }
  _INTR_INLINE static _INTR_ARRAY(Name) & _descFlags(SpecularProbeRef p_Ref)
  {
    return _data.descFlags[p_Ref._id];
  }
  _INTR_INLINE static _INTR_ARRAY(Name) &
      _descSpecularTextureNames(SpecularProbeRef p_Ref)
  {
    return _data.descSpecularTextureNames[p_Ref._id];
  }

  // Resources
  _INTR_INLINE static uint32_t& _flags(SpecularProbeRef p_Ref)
  {
    return _data.flags[p_Ref._id];
  }
};
}
}
}
