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
// Generated from file `_SpiderOperations.java'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

package blackmesa.testlab.devices;

public interface _SpiderOperations extends _AnimalLikeOperations
{
    void move(String password, float direction, float speed, int movementType, Ice.Current __current)
        throws WrongPasswordException;
}
