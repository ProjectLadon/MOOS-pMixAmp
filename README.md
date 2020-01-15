# MOOS-pMixAmp

Mixer/Amplifier block for MOOS. Each mux/amp channel

## Dependencies
* MOOS-IvP
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse

## Configuration Options
* mixer -- defines a mixer with one or more inputs using the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
	"id": "MixAmp_block",
	"type": "object",
	"properties": {
		"inputs": {
			"type":"array",
			"items": {
				"type":"object",
				"properties": {
					"name":{"type":"string"},
					"offset":{"type":"number"},
					"gain":{"type":"number"}
				},
				"required":["name"]
			}
		},
		"output": {
			"type":"object",
			"properties":{
				"name":{"type":"string"},
				"offset":{"type":"number"},
				"gain":{"type":"number"}
			},
			"required":["name"]
		},
		"required":["inputs", "output"]
	}
}
```
