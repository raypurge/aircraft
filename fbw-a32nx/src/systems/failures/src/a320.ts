// Copyright (c) 2021-2023 FlyByWire Simulations
//
// SPDX-License-Identifier: GPL-3.0

import { FailureDefinition } from '@flybywiresim/fbw-sdk';

export const A320Failure = Object.freeze({
  Acsc1Lane1: 21000,
  Acsc1Lane2: 21001,
  Acsc2Lane1: 21002,
  Acsc2Lane2: 21003,
  HotAir: 21004,
  TrimAirHighPressure: 21005,
  CkptTrimAirFailure: 21006,
  FwdTrimAirFailure: 21007,
  AftTrimAirFailure: 21008,
  CkptDuctOvht: 21009,
  FwdDuctOvht: 21010,
  AftDuctOvht: 21011,
  CabinFan1Failure: 21012,
  CabinFan2Failure: 21013,
  LabGalleyFan: 21014,
  Cpc1: 21015,
  Cpc2: 21016,
  OutflowValve: 21017,
  SafetyValve: 21018,
  RapidDecompression: 21019,

  Fac1Failure: 22000,
  Fac2Failure: 22001,

  TransformerRectifier1: 24000,
  TransformerRectifier2: 24001,
  TransformerRectifierEssential: 24002,
  StaticInverter: 24004,
  Generator1: 24020,
  Generator2: 24021,
  ApuGenerator1: 24030,
  AlternatingCurrent1: 24100,
  AlternatingCurrent2: 24101,
  AlternatingCurrentEssential: 24102,
  AlternatingCurrentEssentialShed: 24103,
  AlternatingCurrentStaticInverter: 24104,
  AlternatingCurrentGndFltService: 24105,
  DirectCurrent1: 24106,
  DirectCurrent2: 24107,
  DirectCurrentEssential: 24108,
  DirectCurrentEssentialShed: 24109,
  DirectCurrentBattery: 24110,
  DirectCurrentHot1: 24111,
  DirectCurrentHot2: 24112,
  DirectCurrentGndFltService: 24113,

  Elac1Failure: 27000,
  Elac2Failure: 27001,
  Sec1Failure: 27002,
  Sec2Failure: 27003,
  Sec3Failure: 27004,
  Fcdc1Failure: 27005,
  Fcdc2Failure: 27006,

  GreenReservoirLeak: 29000,
  BlueReservoirLeak: 29001,
  YellowReservoirLeak: 29002,
  GreenReservoirAirLeak: 29003,
  BlueReservoirAirLeak: 29004,
  YellowReservoirAirLeak: 29005,
  GreenReservoirReturnLeak: 29006,
  BlueReservoirReturnLeak: 29007,
  YellowReservoirReturnLeak: 29008,
  GreenEdpOverheat: 29009,
  BlueEpumpOverheat: 29010,
  YellowEdpOverheat: 29011,
  YellowEpumpOverheat: 29012,

  LeftPfdDisplay: 31000,
  RightPfdDisplay: 31001,

  LgciuPowerSupply1: 32000,
  LgciuPowerSupply2: 32001,
  LgciuInternalError1: 32002,
  LgciuInternalError2: 32003,

  GearProxSensorDamageGearUplockNose1: 32004,
  GearProxSensorDamageGearDownlockNose2: 32005,
  GearProxSensorDamageGearUplockRight1: 32006,
  GearProxSensorDamageGearDownlockRight2: 32007,
  GearProxSensorDamageGearUplockLeft2: 32008,
  GearProxSensorDamageGearDownlockLeft1: 32009,

  GearProxSensorDamageGearDoorClosedNose1: 32010,
  GearProxSensorDamageGearDoorOpenedNose2: 32011,
  GearProxSensorDamageGearDoorClosedRight2: 32012,
  GearProxSensorDamageGearDoorOpenedRight1: 32013,
  GearProxSensorDamageGearDoorClosedLeft2: 32014,
  GearProxSensorDamageGearDoorOpenedLeft1: 32015,

  GearActuatorJammedGearNose: 32020,
  GearActuatorJammedGearLeft: 32021,
  GearActuatorJammedGearRight: 32022,
  GearActuatorJammedGearDoorNose: 32023,
  GearActuatorJammedGearDoorLeft: 32024,
  GearActuatorJammedGearDoorRight: 32025,

  GreenBrakeHydraulicLeak: 32100,
  YellowBrakeHydraulicLeak: 32101,
  YellowBrakeAccumulatorGasLeak: 32150,

  RadioAltimeter1: 34000,
  RadioAltimeter2: 34001,
});

export const A320FailureDefinitions: FailureDefinition[] = [
  [21, A320Failure.Acsc1Lane1, 'ACSC 1 Lane 1'],
  [21, A320Failure.Acsc1Lane2, 'ACSC 1 Lane 2'],
  [21, A320Failure.Acsc2Lane1, 'ACSC 2 Lane 1'],
  [21, A320Failure.Acsc2Lane2, 'ACSC 2 Lane 2'],
  [21, A320Failure.HotAir, 'Trim Air Pressure Regulating Valve'],
  [21, A320Failure.TrimAirHighPressure, 'Trim Air System High Pressure'],
  [21, A320Failure.CkptTrimAirFailure, 'Cockpit Trim Air Valve'],
  [21, A320Failure.FwdTrimAirFailure, 'Forward Zone Trim Air Valve'],
  [21, A320Failure.AftTrimAirFailure, 'Aft Zone Trim Air Valve'],
  [21, A320Failure.CkptDuctOvht, 'Cockpit Duct Overheat'],
  [21, A320Failure.FwdDuctOvht, 'Forward Zone Duct Overheat'],
  [21, A320Failure.AftDuctOvht, 'Aft Zone Duct Overheat'],
  [21, A320Failure.CabinFan1Failure, 'Cabin Fan 1'],
  [21, A320Failure.CabinFan2Failure, 'Cabin Fan 2'],
  [21, A320Failure.LabGalleyFan, 'Extraction Fan of lavatory and galley'],
  [21, A320Failure.Cpc1, 'Cabin Pressure Controller 1'],
  [21, A320Failure.Cpc2, 'Cabin Pressure Controller 2'],
  [21, A320Failure.OutflowValve, 'Outflow Valve'],
  [21, A320Failure.SafetyValve, 'Safety Valve'],
  [21, A320Failure.RapidDecompression, 'Rapid Decompression'],

  [22, A320Failure.Fac1Failure, 'FAC 1'],
  [22, A320Failure.Fac2Failure, 'FAC 2'],

  [24, A320Failure.TransformerRectifier1, 'TR 1'],
  [24, A320Failure.TransformerRectifier2, 'TR 2'],
  [24, A320Failure.TransformerRectifierEssential, 'ESS TR'],
  [24, A320Failure.StaticInverter, 'Static Inverter'],
  [24, A320Failure.Generator1, 'Generator 1'],
  [24, A320Failure.Generator2, 'Generator 2'],
  [24, A320Failure.ApuGenerator1, 'APU Generator'],
  [24, A320Failure.AlternatingCurrent1, 'AC 1'],
  [24, A320Failure.AlternatingCurrent2, 'AC 2'],
  [24, A320Failure.AlternatingCurrentEssential, 'AC ESS'],
  [24, A320Failure.AlternatingCurrentEssentialShed, 'AC ESS SCHED'],
  [24, A320Failure.AlternatingCurrentStaticInverter, 'AC STAT INV'],
  [24, A320Failure.AlternatingCurrentGndFltService, 'AC GND FLT SRV'],
  [24, A320Failure.DirectCurrent1, 'DC 1'],
  [24, A320Failure.DirectCurrent2, 'DC 2'],
  [24, A320Failure.DirectCurrentEssential, 'DC ESS'],
  [24, A320Failure.DirectCurrentEssentialShed, 'DC ESS SCHED'],
  [24, A320Failure.DirectCurrentBattery, 'DC BAT'],
  [24, A320Failure.DirectCurrentHot1, 'DC HOT 1'],
  [24, A320Failure.DirectCurrentHot2, 'DC HOT 2'],
  [24, A320Failure.DirectCurrentGndFltService, 'DC GND FLT SRV'],

  [27, A320Failure.Elac1Failure, 'ELAC 1'],
  [27, A320Failure.Elac2Failure, 'ELAC 2'],
  [27, A320Failure.Sec1Failure, 'SEC 1'],
  [27, A320Failure.Sec2Failure, 'SEC 2'],
  [27, A320Failure.Sec3Failure, 'SEC 3'],
  [27, A320Failure.Fcdc1Failure, 'FCDC 1'],
  [27, A320Failure.Fcdc2Failure, 'FCDC 2'],

  [29, A320Failure.GreenReservoirLeak, 'Green reservoir leak'],
  [29, A320Failure.BlueReservoirLeak, 'Blue reservoir leak'],
  [29, A320Failure.YellowReservoirLeak, 'Yellow reservoir leak'],
  [29, A320Failure.GreenReservoirAirLeak, 'Green reservoir air leak'],
  [29, A320Failure.BlueReservoirAirLeak, 'Blue reservoir air leak'],
  [29, A320Failure.YellowReservoirAirLeak, 'Yellow reservoir air leak'],
  [29, A320Failure.GreenReservoirReturnLeak, 'Green reservoir return leak'],
  [29, A320Failure.BlueReservoirReturnLeak, 'Blue reservoir return leak'],
  [29, A320Failure.YellowReservoirReturnLeak, 'Yellow reservoir return leak'],
  [29, A320Failure.GreenEdpOverheat, 'Green engine pump overheat'],
  [29, A320Failure.BlueEpumpOverheat, 'Blue electric pump overheat'],
  [29, A320Failure.YellowEdpOverheat, 'Yellow engine pump overheat'],
  [29, A320Failure.YellowEpumpOverheat, 'Yellow electric pump overheat'],

  [31, A320Failure.LeftPfdDisplay, 'Captain PFD display'],
  [31, A320Failure.RightPfdDisplay, 'F/O PFD display'],

  [32, A320Failure.LgciuPowerSupply1, 'LGCIU 1 Power supply'],
  [32, A320Failure.LgciuPowerSupply2, 'LGCIU 2 Power supply'],
  [32, A320Failure.LgciuInternalError1, 'LGCIU 1 Internal error'],
  [32, A320Failure.LgciuInternalError2, 'LGCIU 2 Internal error'],

  [32, A320Failure.GearProxSensorDamageGearUplockNose1, 'Proximity sensor damage uplock nose gear #1'],
  [32, A320Failure.GearProxSensorDamageGearDownlockNose2, 'Proximity sensor damage downlock nose gear #2'],
  [32, A320Failure.GearProxSensorDamageGearUplockRight1, 'Proximity sensor damage uplock right gear #1'],
  [32, A320Failure.GearProxSensorDamageGearDownlockRight2, 'Proximity sensor damage downlock right gear #2'],
  [32, A320Failure.GearProxSensorDamageGearUplockLeft2, 'Proximity sensor damage uplock left gear #2'],
  [32, A320Failure.GearProxSensorDamageGearDownlockLeft1, 'Proximity sensor damage downlock left gear #1'],
  [32, A320Failure.GearProxSensorDamageGearDoorClosedNose1, 'Proximity sensor damage closed nose gear door #1'],
  [32, A320Failure.GearProxSensorDamageGearDoorOpenedNose2, 'Proximity sensor damage opened nose gear door #2'],
  [32, A320Failure.GearProxSensorDamageGearDoorClosedRight2, 'Proximity sensor damage closed right gear door #2'],
  [32, A320Failure.GearProxSensorDamageGearDoorOpenedRight1, 'Proximity sensor damage opened right gear door #1'],
  [32, A320Failure.GearProxSensorDamageGearDoorClosedLeft2, 'Proximity sensor damage closed left gear door #2'],
  [32, A320Failure.GearProxSensorDamageGearDoorOpenedLeft1, 'Proximity sensor damage opened left gear door #1'],

  [32, A320Failure.GearActuatorJammedGearNose, 'Nose gear jammed actuator'],
  [32, A320Failure.GearActuatorJammedGearLeft, 'Main left gear jammed actuator'],
  [32, A320Failure.GearActuatorJammedGearRight, 'Main right gear jammed actuator'],
  [32, A320Failure.GearActuatorJammedGearDoorNose, 'Nose gear door jammed actuator'],
  [32, A320Failure.GearActuatorJammedGearDoorLeft, 'Main left gear door jammed actuator'],
  [32, A320Failure.GearActuatorJammedGearDoorRight, 'Main right gear door jammed actuator'],

  [32, A320Failure.GreenBrakeHydraulicLeak, 'Green brakes circuit leak'],
  [32, A320Failure.YellowBrakeHydraulicLeak, 'Yellow brakes circuit leak'],
  [32, A320Failure.YellowBrakeAccumulatorGasLeak, 'Yellow brake accumulator gas leak'],

  [34, A320Failure.RadioAltimeter1, 'RA 1'],
  [34, A320Failure.RadioAltimeter2, 'RA 2'],
];
