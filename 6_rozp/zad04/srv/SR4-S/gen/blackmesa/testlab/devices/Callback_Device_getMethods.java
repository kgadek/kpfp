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
// Generated from file `Callback_Device_getMethods.java'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

package blackmesa.testlab.devices;

public abstract class Callback_Device_getMethods extends Ice.TwowayCallback
{
    public abstract void response(Lambda[] __ret);

    public final void __completed(Ice.AsyncResult __result)
    {
        DevicePrx __proxy = (DevicePrx)__result.getProxy();
        Lambda[] __ret = null;
        try
        {
            __ret = __proxy.end_getMethods(__result);
        }
        catch(Ice.LocalException __ex)
        {
            exception(__ex);
            return;
        }
        response(__ret);
    }
}
