
<p align="center">
  <img alt="icon" src="https://raw.githubusercontent.com/Jomart02/Lazuli/9f9eb075cd97fd3005f9697a0e55fc5dc5faa373/gitRes/icon.svg" width="512" height="512">
  </strong>
</p>

<!-- Языки -->
<p align="center">
  <strong style="font-size: 24px;">Language:</strong><br>
  <a href="https://github.com/Jomart02/Lazuli/blob/master/README.md" style="font-size: 20px; text-decoration: none; margin: 0 5px;">🇬🇧</a> • 
  <a href="https://github.com/Jomart02/Lazuli/blob/master/gitRes/README_ru.md" style="font-size: 20px; text-decoration: none; margin: 0 5px;">🇷🇺</a>
</p>


<!-- Навигационное меню -->
<p align="center" style="font-size: 20px;">
  <strong style="font-size: 24px;">Table of Contents:</strong><br>
  <a href="#description">Description</a> • 
  <a href="#features">Features</a> • 
  <a href="#installation">Installation</a> • 
  <a href="#links">Links</a> • 
  <a href="#contributors">Сontributors</a><br>
  <a href="https://github.com/users/Jomart02/projects/1"">
    Roadmap
  </a>
</p>


## Description

Lazuli is versatile NMEA message simulator designed to generate NMEA sentences for testing, development, and educational purposes. It transmits data over UDP, with plans to support serial (COM) port communication in the future. Lazuli stands out with its plugin-based architecture, enabling users to extend its functionality and add custom simulators or new NMEA message types effortlessly.

## Features

### Plugin System for Extensibility:
Lazuli incorporates a robust plugin system, empowering users to expand its capabilities by developing custom plugins. These plugins can introduce new NMEA sentence generators, simulate specific scenarios, or integrate third-party tools, ensuring the application remains flexible and adaptable to evolving needs.

[The base plugin class](https://github.com/Jomart02/BaseNaviWidget/tree/11d4991ce9b7591ce461af3f34f1f4f94c38ff8c) provides the ability to describe the plugin and sends all messages on its own
It is necessary to override one method to implement the generation of NMEA
messages

### Customizable Simulations:
Users can configure simulation profiles to replicate real-world conditions, such as vessel movement, weather data, sensor readings, and more. This customization capability allows developers and testers to validate their systems under diverse and controlled environments.

### Styling Customization with QSS:
Lazuli supports Qt Style Sheets (QSS) and [StyleManager](https://github.com/Jomart02/QssStyleManager/tree/5899cc2385e98501828e1226899dd6fa04698501), enabling users to fully customize the application's visual appearance use custom settings. 

Developed using C++ and Qt 6.5.3's QtWidgets module, Lazuli ensures high performance and cross-platform compatibility.

## Installation 

## Links

## Сontributors