package lab;

/**
* lab/DroneHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from root.idl
* wtorek, 24 kwiecień 2012 14:51:11 CEST
*/

public final class DroneHolder implements org.omg.CORBA.portable.Streamable
{
  public lab.Drone value = null;

  public DroneHolder ()
  {
  }

  public DroneHolder (lab.Drone initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = lab.DroneHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    lab.DroneHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return lab.DroneHelper.type ();
  }

}
