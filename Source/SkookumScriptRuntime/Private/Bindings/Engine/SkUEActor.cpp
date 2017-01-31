//=======================================================================================
// SkookumScript Plugin for Unreal Engine 4
// Copyright (c) 2015 Agog Labs Inc. All rights reserved.
//
// Bindings for the Actor (= AActor) class 
//
// Author: Markus Breyer
//=======================================================================================

//=======================================================================================
// Includes
//=======================================================================================

#include "SkUEActor.hpp"
#include "SkUEEntity.hpp"
#include "../SkUERuntime.hpp"
#include "../SkUEUtils.hpp"
#include "UObjectHash.h"

#include <SkookumScript/SkList.hpp>

//=======================================================================================
// Method Definitions
//=======================================================================================

namespace SkUEActor_Impl
  {

  //---------------------------------------------------------------------------------------
  // Get array of actors of the given class
  static UClass * get_actor_array(TArray<UObject*> * object_array_p, SkClass * class_p)
    {
    UClass * uclass_p = SkUEClassBindingHelper::get_ue_class_from_sk_class(class_p);
    SK_ASSERTX(uclass_p, a_cstr_format("Class '%s' not found. All actor classes must be present. Maybe it is the class of a Blueprint that is not loaded yet?", class_p->get_name_cstr_dbg()));
    if (uclass_p)
      {
      object_array_p->Reserve(1024);
      GetObjectsOfClass(uclass_p, *object_array_p, true, RF_ClassDefaultObject);
      }
    return uclass_p;
    }

  //---------------------------------------------------------------------------------------
  // Find actor of given name (returns nullptr if not found)
  static AActor * find_named(SkInvokedMethod * scope_p, SkClass ** sk_class_pp, UClass ** ue_class_pp)
    {
    // Get actor array
    TArray<UObject*> object_array;
    SkClass * sk_class_p = ((SkMetaClass *)scope_p->get_topmost_scope())->get_class_info();
    UClass * ue_class_p = get_actor_array(&object_array, sk_class_p);

    // Find our actor
    FString name = AStringToFString(scope_p->get_arg<SkString>(SkArg_1));
    UWorld * world_p = SkUEClassBindingHelper::get_world();
    AActor * actor_p = nullptr;
    for (UObject ** RESTRICT obj_pp = object_array.GetData(), **RESTRICT end_pp = obj_pp + object_array.Num(); obj_pp != end_pp; ++obj_pp)
      {
      if ((*obj_pp)->GetWorld() == world_p && (*obj_pp)->GetName() == name)
        {
        actor_p = static_cast<AActor *>(*obj_pp);
        break;
        }
      }

    *sk_class_pp = sk_class_p;
    *ue_class_pp = ue_class_p;

    return actor_p;
    }

  //---------------------------------------------------------------------------------------
  // Make sure a given actor has overlap events enabled on at least one component
#if (SKOOKUM & SK_DEBUG)
  static void assert_actor_has_overlap_events_enabled(AActor * actor_p)
    {
    // Check that events will properly fire
    TArray<UActorComponent *> components = actor_p->GetComponentsByClass(UPrimitiveComponent::StaticClass());
    SK_ASSERTX(components.Num() > 0, a_cstr_format("Trying to receive overlap events on actor '%S' but it has no primitive (collision) component.", *actor_p->GetName()));
    bool found_enabled_overlap_event = false;
    for (UActorComponent * component_p : components)
      {
      if (Cast<UPrimitiveComponent>(component_p)->bGenerateOverlapEvents)
        {
        found_enabled_overlap_event = true;
        break;
        }
      }
    SK_ASSERTX(found_enabled_overlap_event, a_cstr_format("Trying to receive overlap events on actor '%S' but it has no primitive component that has overlap events turned on. To fix this, check the box 'Generate Overlap Events' for the primitive component (e.g. SkeletalMeshComponent, CapsuleComponent etc.) that you would like to trigger the overlap events. You might also simply have picked the wrong actor.", *actor_p->GetName()));
    }
#endif

  //---------------------------------------------------------------------------------------
  // Actor@find_named(Name name) <ThisClass_|None>
  static void mthdc_find_named(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    if (result_pp) // Do nothing if result not desired
      {      
      // Find actor
      SkClass * sk_class_p;
      UClass * ue_class_p;
      AActor * actor_p = find_named(scope_p, &sk_class_p, &ue_class_p);

      // Create instance from our actor, or return nil if none found
      *result_pp = actor_p ? SkUEActor::new_instance(actor_p, ue_class_p, sk_class_p) : SkBrain::ms_nil_p;
      }
    }

  //---------------------------------------------------------------------------------------
  // Actor@named(Name name) <ThisClass_>
  static void mthdc_named(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    if (result_pp) // Do nothing if result not desired
      {
      // Find actor
      SkClass * sk_class_p;
      UClass * ue_class_p;
      AActor * actor_p = find_named(scope_p, &sk_class_p, &ue_class_p);

      #if (SKOOKUM & SK_DEBUG)
        if (!actor_p)
          {
          A_ERRORX(a_str_format("Tried to get instance named '%s' from class '%s', but no such instance exists!\n", scope_p->get_arg<SkString>(SkArg_1).as_cstr(), ((SkMetaClass *)scope_p->get_topmost_scope())->get_class_info()->get_name().as_cstr_dbg()));
          }
      #endif

      // Create instance from our actor, even if null
      *result_pp = SkUEActor::new_instance(actor_p, ue_class_p, sk_class_p);
      }
    }

  //---------------------------------------------------------------------------------------
  // Actor@() List{ThisClass_}
  static void mthdc_instances(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    if (result_pp) // Do nothing if result not desired
      {
      // Get actor array
      TArray<UObject*> object_array;
      SkClass * class_p = ((SkMetaClass *)scope_p->get_topmost_scope())->get_class_info();
      UClass * uclass_p = get_actor_array(&object_array, class_p);

      // Build SkList from it
      UWorld * world_p = SkUEClassBindingHelper::get_world();
      SkInstance * instance_p = SkList::new_instance(object_array.Num());
      SkInstanceList & list = instance_p->as<SkList>();
      APArray<SkInstance> & instances = list.get_instances();
      for (UObject ** RESTRICT obj_pp = object_array.GetData(), **RESTRICT end_pp = obj_pp + object_array.Num(); obj_pp != end_pp; ++obj_pp)
        {
        // Must be in this world and not about to die
        if ((*obj_pp)->GetWorld() == world_p && !(*obj_pp)->IsPendingKill())
          {
          // This instance is already refcounted so directly append to underlying array
          instances.append(*SkUEActor::new_instance(static_cast<AActor *>(*obj_pp), uclass_p, class_p));
          }
        }
      *result_pp = instance_p;
      }
    }

  //---------------------------------------------------------------------------------------
  // Actor@() ThisClass_
  static void mthdc_instances_first(SkInvokedMethod * scope_p, SkInstance ** result_pp)
    {
    if (result_pp) // Do nothing if result not desired
      {
      // Get actor array
      TArray<UObject*> object_array;
      SkClass * class_p = ((SkMetaClass *)scope_p->get_topmost_scope())->get_class_info();
      UClass * uclass_p = get_actor_array(&object_array, class_p);

      // Return first one
      UWorld * world_p = SkUEClassBindingHelper::get_world();
      for (UObject ** RESTRICT obj_pp = object_array.GetData(), **RESTRICT end_pp = obj_pp + object_array.Num(); obj_pp != end_pp; ++obj_pp)
        {
        // Must be in this world and not about to die
        if ((*obj_pp)->GetWorld() == world_p && !(*obj_pp)->IsPendingKill())
          {
          *result_pp = SkUEActor::new_instance(static_cast<AActor *>(*obj_pp), uclass_p, class_p);
          return;
          }
        }

      // None found
      *result_pp = SkBrain::ms_nil_p;
      }
    }

  static const SkClass::MethodInitializerFunc methods_c2[] =
    {
      { "find_named",       mthdc_find_named },
      { "named",            mthdc_named },
      { "instances",        mthdc_instances },
      { "instances_first",  mthdc_instances_first },
    };

  } // SkUEActor_Impl

//---------------------------------------------------------------------------------------

void SkUEActor_Ext::register_bindings()
  {
  ms_class_p->register_method_func_bulk(SkUEActor_Impl::methods_c2, A_COUNT_OF(SkUEActor_Impl::methods_c2), SkBindFlag_class_no_rebind);
  }


