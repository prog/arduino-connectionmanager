#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>



namespace prog { class ConnectionManager
{

	public:

		ConnectionManager();

		void setAP(String name, String password);

		void setClient(String name, String password);

		void unsetAP();

		void unsetClient();

		bool isAPSet();

		bool isAPEnabled();

		bool isClientSet();

		bool isClientEnabled();

		bool isClientConnected();

		void handle();

	private:

		String clientName;
		String clientPassword;
		String apName;
		String apPassword;
		unsigned long apAfter;

		bool apEnabled;
		bool clientEnabled;
		bool clientConnected;
		unsigned long clientStatusSince;

		void enableAP();

		void disableAP();

		void enableClient();

		void disableClient();

}; }
