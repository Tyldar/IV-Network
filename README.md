<p align="center"><img src="http://dev.gta-network.net/content/iv-network.png" align="center" style="width:50%;" /></p>
<p align="center" style="font-size:14px;">An alternative multiplayer modification for GTA IV PC</p>

## Quick start

Three quick start options are available:

* [Download the latest release](https://github.com/IV-Network/IV-Network/archive/master.zip).
* Clone the repo: `https://github.com/IV-Network/IV-Network.git`.
* For compiled builds, visit http://iv-network.net/?page=downloads

Visit [IV:Network forum](http://forum.iv-network.net/) to stay up to date!

## Description

IV: Network (IV:Net) is a modification for Grand Theft Auto: IV (GTA:IV) founded in 2013. It turns GTA:IV into an alternative multiplayer game. You can play over the internet (or LAN). You will need the original GTA:IV PC game in order to play IV:Net.

In IVNet you are able to do almost anything that you can do in single player GTA:IV. IV:Net features the possibility to create your own scripted modes. There are many game modes that feature a series of different and fun things to do. Many of which are not possible in single player.

## Legal

IV: Network (IV:Net) is in no way affiliated with Rockstar Games, Rockstar North or any of Rockstar's subcompanies.

## Info

We accept only pull request if you want to contribute

## Compile on Windows

* You should use [Visual Studio 2013] (http://www.microsoft.com/visualstudio/eng/downloads)
* Open IVN.sln
* Right click on "Solution 'IVN' (XY projects) in Solution Explorer
* Build Solution

## Compile on Linux

Only once:
 On Ubuntu:
* sudo add-apt-repository ppa:ubuntu-toolchain-r/test

 On Debian:
* nano /etc/apt/source.list
deb http://ftp.de.debian.org/debian sid main

 Common:
* sudo apt-get update
* sudo apt-get install g++-4.7 c++-4.7
* sudo update-alternatives --install "/usr/bin/gcc" "gcc" "/usr/bin/gcc-4.7" 1
* sudo update-alternatives --install "/usr/bin/g++" "g++" "/usr/bin/g++-4.7" 1
* sudo apt-get install libc6-dev-i386
* sudo apt-get install gcc-4.7-multilib g++-4.7-multilib
	
On every compile:
* cd path/to/source/
* make