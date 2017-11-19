# 1.0 MOOS Mixer/Amplifier Module {#SRD001 }

This module provides a multi-input mixer/amplifier block.

## 1.1 Software Strucutre & Dependencies {#SRD002 }

### 1.1.1 SRD003 {#SRD003 }

This module **shall** be implemented as a MOOS Appcasting App as describe in the (http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php)[MOOS-IvP wiki].

### 1.1.2 SRD004 {#SRD004 }

This module **shall** use (http://www.rapidjson.org)[rapidjson] as the JSON parse/unparse library.

## 1.2 MOOS Startup Configuration Options {#SRD005 }

### 1.2.1 SRD006 {#SRD006 }

The configuration file **must** have one ```mixer``` entry.

### 1.2.2 SRD007 {#SRD007 }

The ```mixer``` entry **must** be a JSON conforming to schema/mixer_schema.json. If it does not conform, this module **shall** log an error and exit.

### 1.2.3 SRD008 {#SRD008 }

 The document **must** contain an ```inputs``` key, which **must** be a JSON array.

#### 1.2.3.1 SRD009 {#SRD009 }

The elements of this array **must** each have a ```name``` key. This module **shall** subscribe to the named MOOS variable.

#### 1.2.3.2 SRD010 {#SRD010 }

The elements of this array **may** each have an ```offset``` key. If it is not present, its value **shall** default to 0.

#### 1.2.3.3 SRD011 {#SRD011 }

The elements of this array **may** each have a ```gain``` key. If it is not present, its value **shall** default to 1.

### 1.2.4 SRD012 {#SRD012 }

The document **must** contain an ```output```, which **must** be a JSON object.

#### 1.2.4.1 SRD013 {#SRD013 }

This object **must** have a ```name``` key. This module **shall** publish its output to that variable.

#### 1.2.4.2 SRD014 {#SRD014 }

This object **may** each have an ```offset``` key. If it is not present, its value **shall** default to 0.

#### 1.2.4.3 SRD015 {#SRD015 }

This object **may** each have a ```gain``` key. If it is not present, its value **shall** default to 1.

# 2.0 Data Processing {#SRD016 }

## 2.1 SRD017 {#SRD017 }

On each comms tick, this module **shall** read and process all inputs.

## 2.2 SRD018 {#SRD018 }

Each input **must** be a DOUBLE; if it is not, this module **shall** ignore it.

## 2.3 SRD019 {#SRD019 }

This module **shall** add each input's offset to the input and multiply the result by that input's gain.

## 2.4 SRD020 {#SRD020 }

This module **shall** sum all of the processed inputs.

## 2.5 SRD021 {#SRD021 }

This module **shall** add the output offset to the sum and multiply the result by output gain.

## 2.6 SRD022 {#SRD022 }

This module **shall** publish the resulting value immediately.

