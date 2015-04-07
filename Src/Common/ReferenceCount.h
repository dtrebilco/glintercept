/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __REFERENCE_COUNT_H_
#define __REFERENCE_COUNT_H_

//@
//  Summary:
//    This class provides basic reference counting to classes that 
//    inherit from it.
//  
class ReferenceCount
{
public:

  //@
  //  Summary:
  //    Constructor, reference count is init. to 1.
  //  
  inline ReferenceCount();

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

protected:

  //@
  //  Summary:
  //    To delete an object of "this" class. This is called when the reference
  //    count reaches 0. 
  //  
  inline virtual void DeleteThis() const;

  //@
  //  Summary:
  //    Destructor. This should only be called from DeleteThis and should
  //    remain protected in descendent classes.
  //  
  inline virtual ~ReferenceCount();

private:

  mutable int referenceCount;                     // The reference count of this object. 
                                                  // The object is deleted when this becomes 0
};

///////////////////////////////////////////////////////////////////////////////
//
inline ReferenceCount::ReferenceCount():
referenceCount(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ReferenceCount::AddReference() const
{
  referenceCount++;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ReferenceCount::ReleaseReference() const
{
  referenceCount--;

  //Delete this object if the reference count reaches 0
  if(referenceCount == 0)
  {
    DeleteThis();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ReferenceCount::DeleteThis() const
{
  //Delete this pointer
  delete this;
}

///////////////////////////////////////////////////////////////////////////////
//
inline ReferenceCount::~ReferenceCount()
{
  //This destructor only exists to enforce a virtual protected destructor
  // in descendent classes
}

#endif // __REFERENCE_COUNT_H_
