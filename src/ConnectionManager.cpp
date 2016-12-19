#include "ConnectionManager.hpp"

using prog::ConnectionManager;



ConnectionManager::ConnectionManager()
{
	this->clientStatusSince = millis();
	this->clientConnected = false;
	this->clientEnabled = false;
	this->apEnabled = false;
	this->apAfter = 30 * 1000;
}


void ConnectionManager::setClient(String name, String password)
{
	this->clientName = name;
	this->clientPassword = password;
	if (this->isClientEnabled()) {
		this->disableClient();
	}
}


void ConnectionManager::setAP(String name, String password)
{
	this->apName = name;
	this->apPassword = password;
	if (this->isAPEnabled()) {
		this->disableAP();
	}
}


void ConnectionManager::unsetClient()
{
	this->setClient("", "");
}


void ConnectionManager::unsetAP()
{
	this->setAP("", "");
}


bool ConnectionManager::isAPSet()
{
	return (this->apName != "");
}


bool ConnectionManager::isAPEnabled()
{
	return this->apEnabled;
}


bool ConnectionManager::isClientSet()
{
	return (this->clientName != "");
}


bool ConnectionManager::isClientEnabled()
{
	return this->clientEnabled;
}


bool ConnectionManager::isClientConnected()
{
	return this->clientConnected;
}


void ConnectionManager::handle()
{
	unsigned long now = millis();
	bool isConnected = (this->clientEnabled && WL_CONNECTED == WiFi.status());
	if (isConnected != this->clientConnected) {
		this->clientConnected = isConnected;
		this->clientStatusSince = now;
		// Serial.println(isConnected ? "client connected" : "client disconnected");
	}

	if (this->clientEnabled) {
		if (!this->clientConnected && !this->apEnabled) {
			if ((now - this->clientStatusSince) > this->apAfter && this->isAPSet()) {
				this->enableAP();
			}
		} else if (this->clientConnected && this->apEnabled) {
			this->disableAP();
		}
	} else if (this->isClientSet()) {
		this->enableClient();
	} else if (!this->apEnabled && this->apName != "") {
		this->enableAP();
	}
}


void ConnectionManager::enableClient()
{
	// Serial.println("enable client");
	WiFi.mode(this->apEnabled ? WIFI_AP_STA : WIFI_STA);
	WiFi.begin(this->clientName.c_str(), this->clientPassword.c_str());
	this->clientConnected = false;
	this->clientEnabled = true;
	this->clientStatusSince = millis();
}


void ConnectionManager::disableClient()
{
	// Serial.println("disable client");
	WiFi.disconnect();
	WiFi.mode(this->apEnabled ? WIFI_AP : WIFI_OFF);
	this->clientEnabled = false;
	this->clientConnected = false;
	this->clientStatusSince = millis();
}


void ConnectionManager::enableAP()
{
	// Serial.println("enable AP");
	WiFi.mode(this->clientEnabled ? WIFI_AP_STA : WIFI_AP);
	WiFi.softAP(this->apName.c_str(), this->apPassword.c_str());
	this->apEnabled = true;
}


void ConnectionManager::disableAP()
{
	// Serial.println("disable AP");
	WiFi.softAPdisconnect();
	WiFi.mode(this->clientEnabled ? WIFI_STA : WIFI_OFF);
	this->apEnabled = false;
}
