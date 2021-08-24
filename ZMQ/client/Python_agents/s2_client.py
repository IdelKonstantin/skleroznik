import json
import zmq
import argparse
import ast
import S2_solver_API

Meteo = {
	'T': 15,
	'P': 1000,
	'H': 50,
	'windSpeed': 4.0,
	'windDir': 90,
}

Bullet = {
	'bulletName': '7N1 (7N14)',
	'dragFunction': S2_solver_API.dragModels['G7'],
	'BC': 0.211,
	'DSF_0_9': 1.000,
	'DSF_1_0': 1.000,
	'DSF_1_1': 1.000,
	'V0': 830,
	'length': 32.1,
	'mass': 151,
	'calinber': 7.8,
	'V0temp': 15,
	'thermalSens': 0.7,
	}

Rifle = {
	'rifleName': 'SND (twist = 240 mm)',
	'zeroDistance': 100,
	'scopeHight': 5.0,
	'twist': 240.0,
	'twistDir': S2_solver_API.twistDirection['RIGHT_TWIST'],
	'zeroAtm': S2_solver_API.zeroAtmosphere['HERE'],
	'zeroTemp': 15,
	'zeroPress': 1000,
	'vertDrift': 0.0,
	'vertDrDir': S2_solver_API.POIDrift['POI_RIGHT'],
	'horizDrift': 0.0,
	'horizDrDir': S2_solver_API.POIDrift['POI_DOWN'],
	}

Scope = {
	'scopeName': 'Dedal-NV scope',
	'angleUnits': S2_solver_API.angleUnits['MRAD_UNITS'],
	'clickVert': 0.100,
	'clickHoriz': 0.100,
	'reticlePattern': S2_solver_API.reticlePatterns['MIL_DOT'],
	}

Inputs = {
	'shotDistance': 600,
	'terrainAndle': 0,
	'targetSpeedInMILs': 0.0,
	'targetAzimuth': 15,
	'latitude': 56.0,
	'magneticIncl': 5.0,
	}

Options = {
	'Koriolis': S2_solver_API.inputOptions['OPTION_NO'],
	'BallisticTable': S2_solver_API.inputOptions['OPTION_NO'],
	'ThermalCorrection': S2_solver_API.inputOptions['OPTION_NO'],
	}

data = {
	'Meteo': Meteo,
	'Bullet': Bullet,
	'Rifle': Rifle,
	'Scope': Scope,
	'Inputs': Inputs,
	'Options': Options,
	}

json_str = json.dumps(data)

###############################################
parser = argparse.ArgumentParser(description='S2 ZMQ service')

parser.add_argument('--host', action='store', dest='host',
        help='host IP of S2 server',
        default='localhost')

parser.add_argument('--port', action='store', dest='port', 
        help='port of S2 service',
	default='5555')

host = parser.parse_args().host
port = parser.parse_args().port
###############################################
 
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://" + host + ":" + port)
socket.send(json_str.encode('UTF-8'))

message = socket.recv()
ballisticData = ast.literal_eval(bytes.decode(message, encoding='utf-8'))

print("Vertical -------> " + str(ballisticData['Vertical']))
print("Horizontal -----> " + str(ballisticData['Horizontal']))
print("Derivation -----> " + str(ballisticData['Derivation']))
print("Time -----------> " + str(ballisticData['Time']))
print("Cinetic Energy -> " + str(ballisticData['Cinetic Energy']))
print("Mach Number ----> " + str(ballisticData['Mach Number']))
print("Miller's FGS ---> " + str(ballisticData['Miller\'s FGS']))
