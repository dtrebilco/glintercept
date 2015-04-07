/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __AUTO_REFERENCE_COUNT_H_
#define __AUTO_REFERENCE_COUNT_H_

//@
//  Summary:
//    This template class provides a wrapper to auto increment/decrement
//    reference counted objects.
//  
template <class T>
class AutoReferenceCount
{
public:

  //@
  //  Summary:
  //    Constructor
  //
  //  Parameters:
  //    copy  - A existing AutoRefernceCount object to take the 
  //            reference counted object from.
  // 
  inline AutoReferenceCount();
  inline AutoReferenceCount(const AutoReferenceCount<T> &copy);

  //@
  //  Summary:
  //    Destructor. Releases any internal reference counted object.
  //
  inline virtual ~AutoReferenceCount();

  //@
  //  Summary:
  //    To attach a reference counted object to this object.
  //    This class now "ownes" the passed object and the caller 
  //    should not release reference on it.
  //
  //  Parameters:
  //    newObj  - The new object to auto reference count.
  // 
  inline void Attach(T *newObj);

  //@
  //  Summary:
  //    To detach a reference counted object to this object.
  //    It is now the responsibility of the caller to release the returned
  //    object when necessary. 
  //
  //  Returns:
  //    The object that was previously auto referenced (if any) is returned
  //    to the caller.
  // 
  inline T *Detach(); 

  //@
  //  Summary:
  //    To compare this auto reference object with another auto reference
  //    object.
  //
  //  Parameters:
  //    compare  - The object to compare with.
  // 
  inline bool operator==(const AutoReferenceCount<T> &compare) const;
  inline bool operator!=(const AutoReferenceCount<T> &compare) const;

  //@
  //  Summary:
  //    To assign another auto reference object to this object. Both 
  //    auto references will now track the internal object. (if any)
  //
  //  Parameters:
  //    assign  - The object to now track.
  // 
  inline AutoReferenceCount &operator=(const AutoReferenceCount<T> &assign);


  //@
  //  Summary:
  //    To get the internal tracked object.
  //
  //  Rerurns:
  //    The internal tracked object is returned.
  // 
  inline T &operator*();
  inline const T &operator*() const;
  
  inline T *operator->();
  inline const T *operator->() const;

  //@
  //  Summary:
  //    To return if the there is a internal object being referenced. 
  //
  //  Rerurns:
  //    If there is a object being referenced, true is returned. Else
  //    false is returned.
  // 
  inline operator bool() const;

  //@
  //  Summary:
  //    To return if the there is not a internal object being referenced. 
  //
  //  Rerurns:
  //    If there is a object being referenced, false is returned. Else
  //    true is returned.
  // 
  inline bool operator!() const;

private:

  T *refObj;                                      // The auto reference counted object

  //@
  //  Summary:
  //    Increase the reference count of the class by 1.
  //  
  inline void AddReference() const;

  //@
  //  Summary:
  //    Decrease the reference count of the class by 1.
  //    If the reference count reaches 0, the class is deleted.
  //  
  inline void ReleaseReference() const;

};

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline AutoReferenceCount<T>::AutoReferenceCount():
refObj(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline AutoReferenceCount<T>::AutoReferenceCount(const AutoReferenceCount<T> &copy):
refObj(copy.refObj)
{
  AddReference();
}


///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline AutoReferenceCount<T>::~AutoReferenceCount()
{
  //Release a reference on the contained object (if any)
  ReleaseReference();
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline bool AutoReferenceCount<T>::operator==(const AutoReferenceCount<T> &compare) const
{
  return (refObj == compare.refObj);
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline bool AutoReferenceCount<T>::operator!=(const AutoReferenceCount<T> &compare) const
{
  return (refObj != compare.refObj);
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline AutoReferenceCount<T> &AutoReferenceCount<T>::operator=(const AutoReferenceCount<T> &assign)
{
  //Check that it is not a self assignment
  if(refObj != assign.refObj)
  {
    //Release any existing object
    ReleaseReference();
    
    //Assign the object
    refObj = assign.refObj;

    //Add a reference to the new object (if any)
    AddReference();
  }

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline T &AutoReferenceCount<T>::operator*()
{
  return *refObj;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline const T &AutoReferenceCount<T>::operator*() const
{
  return *refObj;  
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline T *AutoReferenceCount<T>::operator->()
{
  return refObj;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline const T *AutoReferenceCount<T>::operator->() const
{
  return refObj;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline AutoReferenceCount<T>::operator bool() const
{
  return refObj != NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline bool AutoReferenceCount<T>::operator!() const
{
  return refObj == NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline void AutoReferenceCount<T>::Attach(T *newObj) 
{
  //Release any existing reference
  ReleaseReference();

  //This class now "owns" this object
  refObj = newObj;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline T *AutoReferenceCount<T>::Detach() 
{
  //Get the object to return
  T * returnObj = refObj;
  
  //NULL out our reference
  refObj = NULL;

  //Return the object
  return returnObj;
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline void AutoReferenceCount<T>::AddReference() const
{
  //If there is a contained object, add a reference
  if(refObj)
  {
    refObj->AddReference();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline void AutoReferenceCount<T>::ReleaseReference() const
{
  //If there is a contained object, release a reference
  if(refObj)
  {
    refObj->ReleaseReference();
  }
}

#endif // __AUTO_REFERENCE_COUNT_H_
