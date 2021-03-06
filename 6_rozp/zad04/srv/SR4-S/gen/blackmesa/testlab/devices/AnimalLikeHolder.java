// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.4.2
//
// <auto-generated>
//
// Generated from file `AnimalLikeHolder.java'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

package blackmesa.testlab.devices;

public final class AnimalLikeHolder extends Ice.ObjectHolderBase<AnimalLike>
{
    public
    AnimalLikeHolder()
    {
    }

    public
    AnimalLikeHolder(AnimalLike value)
    {
        this.value = value;
    }

    public void
    patch(Ice.Object v)
    {
        try
        {
            value = (AnimalLike)v;
        }
        catch(ClassCastException ex)
        {
            IceInternal.Ex.throwUOE(type(), v.ice_id());
        }
    }

    public String
    type()
    {
        return _AnimalLikeDisp.ice_staticId();
    }
}
