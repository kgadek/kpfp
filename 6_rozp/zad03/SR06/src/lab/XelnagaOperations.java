package lab;


/**
* lab/xelnagaOperations.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from root.idl
* wtorek, 24 kwiecie� 2012 02:57:52 CEST
*/

public interface XelnagaOperations 
{

  // stupid CORBA, Y U can't put this inline?
  int xid ();

  // stupid CORBA, Y U can't put this inline?
  void xid (int newXid);
  lab.Drone searchDroneByName (String name);
  lab.Drone[] searchSpaceAndDestroyProtossRace ();
  void controlDrone (int id, lab.Overlord lstnr);
  void abandonDrone (int id, lab.Overlord lstnr);
} // interface xelnagaOperations
