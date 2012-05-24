// see half-life.wikia.com/wikia/Black_Mesa_Incident

module blackmesa {
	module scientist {
		// -[ on reports ]-------------------------------------------------------------
		struct Report {
			int importance;
			string msg;
		};

		// -[ on spectators ]----------------------------------------------------------
		interface Spectator {
			void acknowledge(Report rprt);
		};
	};

	// ================================================================================
	module devices {
		// -[ on lambdas ]-------------------------------------------------------------
		enum Type { TInteger, TFloat, TNone };
		sequence<Type> Types;
		struct Lambda {
			string name;
			Type return;
			Types params;
		};
		sequence<Lambda> Lambdas;

		// -[ on devices ]-------------------------------------------------------------
		interface Device {
			Lambdas functions(); // get functions
			void takeover(string login); // start controlling
			void release(string login); // stop controlling
			void observe(string login, scientist::Spectator* observer); // start observing
			void ignore(string login, scientist::Spectator* observer); // stop observing
		};
	};

	// ================================================================================
	module testlab {
		// -[ on test lab ]------------------------------------------------------------
		sequence<string> Strings;
		struct DeviceAvailable {
			string type; // in java terms: a class of a machine
			Strings machines; // machines of a given type
		};
		sequence<DeviceAvailable> DevicesAvailable;
		interface BlackMesaTestLab {
			DevicesAvailable list();
			// stupid structs, U Y cannot be forward declared? ;<
			// stupid structs, U Y cannot be defined elswhere? ;<
		};
	};
};

