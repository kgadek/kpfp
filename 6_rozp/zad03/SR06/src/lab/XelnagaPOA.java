package lab;


/**
* lab/xelnagaPOA.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from root.idl
* wtorek, 24 kwiecie� 2012 02:57:52 CEST
*/

@SuppressWarnings("unchecked")
public abstract class XelnagaPOA extends org.omg.PortableServer.Servant
 implements lab.XelnagaOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  @SuppressWarnings("rawtypes")
private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("_get_xid", new java.lang.Integer (0));
    _methods.put ("_set_xid", new java.lang.Integer (1));
    _methods.put ("searchDroneByName", new java.lang.Integer (2));
    _methods.put ("searchSpaceAndDestroyProtossRace", new java.lang.Integer (3));
    _methods.put ("controlDrone", new java.lang.Integer (4));
    _methods.put ("abandonDrone", new java.lang.Integer (5));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {

  // stupid CORBA, Y U can't put this inline?
       case 0:  // lab/xelnaga/_get_xid
       {
         int $result = (int)0;
         $result = this.xid ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }


  // stupid CORBA, Y U can't put this inline?
       case 1:  // lab/xelnaga/_set_xid
       {
         int newXid = in.read_long ();
         this.xid (newXid);
         out = $rh.createReply();
         break;
       }

       case 2:  // lab/xelnaga/searchDroneByName
       {
         String name = in.read_string ();
         lab.Drone $result = null;
         $result = this.searchDroneByName (name);
         out = $rh.createReply();
         lab.DroneHelper.write (out, $result);
         break;
       }

       case 3:  // lab/xelnaga/searchSpaceAndDestroyProtossRace
       {
         lab.Drone $result[] = null;
         $result = this.searchSpaceAndDestroyProtossRace ();
         out = $rh.createReply();
         lab.xelnagaPackage.armyOfDronesHelper.write (out, $result);
         break;
       }

       case 4:  // lab/xelnaga/controlDrone
       {
         int id = in.read_long ();
         lab.Overlord lstnr = lab.OverlordHelper.read (in);
         this.controlDrone (id, lstnr);
         out = $rh.createReply();
         break;
       }

       case 5:  // lab/xelnaga/abandonDrone
       {
         int id = in.read_long ();
         lab.Overlord lstnr = lab.OverlordHelper.read (in);
         this.abandonDrone (id, lstnr);
         out = $rh.createReply();
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:lab/xelnaga:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public Xelnaga _this() 
  {
    return XelnagaHelper.narrow(
    super._this_object());
  }

  public Xelnaga _this(org.omg.CORBA.ORB orb) 
  {
    return XelnagaHelper.narrow(
    super._this_object(orb));
  }


} // class xelnagaPOA
