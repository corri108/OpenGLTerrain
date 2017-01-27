/*
* PerlinNoise.h
*
*  Created on: Jan 24, 2017
*      Author: socce_000
*/

#pragma once

class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);
	double GetHeight(double x, double y);

	// Get
	double Persistence() const { return persistence; }
	double Frequency()   const { return frequency; }
	double Amplitude()   const { return amplitude; }
	int    Octaves()     const { return octaves; }
	int    RandomSeed()  const { return randomseed; }

	// Set
	void Set(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);
	void SetPersistence(double _persistence) { persistence = _persistence; }
	void SetFrequency(double _frequency) { frequency = _frequency; }
	void SetAmplitude(double _amplitude) { amplitude = _amplitude; }
	void SetOctaves(int    _octaves) { octaves = _octaves; }
	void SetRandomSeed(int    _randomseed) { randomseed = _randomseed; }

private:
	double Total(double i, double j);
	double GetValue(double x, double y);
	double Interpolate(double x, double y, double a);
	double Noise(int x, int y);

	double persistence, frequency, amplitude;
	int octaves, randomseed;
};
