#include "FlyByWire.h"
#include "FlyByWire_private.h"
#include "look1_binlxpw.h"
#include "look2_binlxpw.h"

const uint8_T FlyByWire_IN_InAir = 1U;
const uint8_T FlyByWire_IN_OnGround = 2U;
const uint8_T FlyByWire_IN_Flying = 1U;
const uint8_T FlyByWire_IN_Landed = 2U;
const uint8_T FlyByWire_IN_Landing100ft = 3U;
const uint8_T FlyByWire_IN_Takeoff100ft = 4U;
const uint8_T FlyByWire_IN_Flare_Reduce_Theta_c = 1U;
const uint8_T FlyByWire_IN_Flare_Set_Rate = 2U;
const uint8_T FlyByWire_IN_Flare_Store_Theta_c_deg = 3U;
const uint8_T FlyByWire_IN_Flight_High = 4U;
const uint8_T FlyByWire_IN_Flight_Low = 5U;
const uint8_T FlyByWire_IN_Ground = 6U;
const uint8_T FlyByWire_IN_frozen = 1U;
const uint8_T FlyByWire_IN_running = 2U;
const uint8_T FlyByWire_IN_Flight = 1U;
const uint8_T FlyByWire_IN_FlightToGroundTransition = 2U;
const uint8_T FlyByWire_IN_Ground_p = 3U;
const uint8_T FlyByWire_IN_automatic = 1U;
const uint8_T FlyByWire_IN_manual = 2U;
const uint8_T FlyByWire_IN_reset = 3U;
const uint8_T FlyByWire_IN_tracking = 4U;
const uint8_T FlyByWire_IN_flight_clean = 1U;
const uint8_T FlyByWire_IN_flight_flaps = 2U;
const uint8_T FlyByWire_IN_ground = 3U;
const uint8_T FlyByWire_IN_OFF = 1U;
const uint8_T FlyByWire_IN_ON = 2U;
const uint8_T FlyByWire_IN_FlightMode = 1U;
const uint8_T FlyByWire_IN_GroundMode = 2U;
void FlyByWireModelClass::FlyByWire_GetIASforMach3(real_T rtu_m, real_T rtu_m_t, real_T rtu_v, real_T *rty_v_t)
{
  *rty_v_t = rtu_v * rtu_m_t / rtu_m;
}

void FlyByWireModelClass::FlyByWire_GetIASforMach4(real_T rtu_m, real_T rtu_m_t, real_T rtu_v, real_T *rty_v_t)
{
  *rty_v_t = rtu_v * rtu_m_t / rtu_m;
}

void FlyByWireModelClass::FlyByWire_LagFilter(real_T rtu_U, real_T rtu_C1, real_T rtu_dt, real_T *rty_Y,
  rtDW_LagFilter_FlyByWire_T *localDW)
{
  real_T ca;
  real_T denom_tmp;
  if ((!localDW->pY_not_empty) || (!localDW->pU_not_empty)) {
    localDW->pU = rtu_U;
    localDW->pU_not_empty = true;
    localDW->pY = rtu_U;
    localDW->pY_not_empty = true;
  }

  denom_tmp = rtu_dt * rtu_C1;
  ca = denom_tmp / (denom_tmp + 2.0);
  *rty_Y = (2.0 - denom_tmp) / (denom_tmp + 2.0) * localDW->pY + (rtu_U * ca + localDW->pU * ca);
  localDW->pY = *rty_Y;
  localDW->pU = rtu_U;
}

void FlyByWireModelClass::FlyByWire_TimeSinceCondition(real_T rtu_time, boolean_T rtu_condition, real_T *rty_y,
  rtDW_TimeSinceCondition_FlyByWire_T *localDW)
{
  if (!localDW->eventTime_not_empty) {
    localDW->eventTime = rtu_time;
    localDW->eventTime_not_empty = true;
  }

  if ((!rtu_condition) || (localDW->eventTime == 0.0)) {
    localDW->eventTime = rtu_time;
  }

  *rty_y = rtu_time - localDW->eventTime;
}

void FlyByWireModelClass::FlyByWire_RateLimiter(real_T rtu_u, real_T rtu_up, real_T rtu_lo, real_T rtu_Ts, real_T
  rtu_init, real_T *rty_Y, rtDW_RateLimiter_FlyByWire_T *localDW)
{
  real_T u0;
  real_T u1;
  if (!localDW->pY_not_empty) {
    localDW->pY = rtu_init;
    localDW->pY_not_empty = true;
  }

  u0 = rtu_u - localDW->pY;
  u1 = rtu_up * rtu_Ts;
  if (u0 < u1) {
    u1 = u0;
  }

  u0 = rtu_lo * rtu_Ts;
  if (u1 > u0) {
    u0 = u1;
  }

  localDW->pY += u0;
  *rty_Y = localDW->pY;
}

void FlyByWireModelClass::FlyByWire_WashoutFilter(real_T rtu_U, real_T rtu_C1, real_T rtu_dt, real_T *rty_Y,
  rtDW_WashoutFilter_FlyByWire_T *localDW)
{
  real_T ca;
  real_T denom_tmp;
  if ((!localDW->pY_not_empty) || (!localDW->pU_not_empty)) {
    localDW->pU = rtu_U;
    localDW->pU_not_empty = true;
    localDW->pY = rtu_U;
    localDW->pY_not_empty = true;
  }

  denom_tmp = rtu_dt * rtu_C1;
  ca = 2.0 / (denom_tmp + 2.0);
  *rty_Y = (2.0 - denom_tmp) / (denom_tmp + 2.0) * localDW->pY + (rtu_U * ca - localDW->pU * ca);
  localDW->pY = *rty_Y;
  localDW->pU = rtu_U;
}

void FlyByWireModelClass::FlyByWire_eta_trim_limit_lofreeze(real_T rtu_eta_trim, real_T rtu_trigger, real_T *rty_y,
  rtDW_eta_trim_limit_lofreeze_FlyByWire_T *localDW)
{
  if ((rtu_trigger == 0.0) || (!localDW->frozen_eta_trim_not_empty)) {
    localDW->frozen_eta_trim = rtu_eta_trim;
    localDW->frozen_eta_trim_not_empty = true;
  }

  *rty_y = localDW->frozen_eta_trim;
}

void FlyByWireModelClass::FlyByWire_ConvertToEuler(real_T rtu_Theta, real_T rtu_Phi, real_T rtu_q, real_T rtu_r, real_T
  rtu_p, real_T *rty_qk, real_T *rty_rk, real_T *rty_pk)
{
  real_T tmp[9];
  real_T result[3];
  real_T Phi;
  real_T Theta;
  real_T result_tmp;
  real_T result_tmp_0;
  int32_T i;
  Theta = 0.017453292519943295 * rtu_Theta;
  Phi = 0.017453292519943295 * rtu_Phi;
  result_tmp = std::tan(Theta);
  result_tmp_0 = std::sin(Phi);
  Phi = std::cos(Phi);
  tmp[0] = 1.0;
  tmp[3] = result_tmp_0 * result_tmp;
  tmp[6] = Phi * result_tmp;
  tmp[1] = 0.0;
  tmp[4] = Phi;
  tmp[7] = -result_tmp_0;
  tmp[2] = 0.0;
  Theta = 1.0 / std::cos(Theta);
  tmp[5] = Theta * result_tmp_0;
  tmp[8] = Theta * Phi;
  for (i = 0; i < 3; i++) {
    result[i] = tmp[i + 6] * rtu_r + (tmp[i + 3] * rtu_q + tmp[i] * rtu_p);
  }

  *rty_qk = result[1];
  *rty_rk = result[2];
  *rty_pk = result[0];
}

void FlyByWireModelClass::FlyByWire_CalculateV_alpha_max(real_T rtu_v_ias, real_T rtu_alpha, real_T rtu_alpha_0, real_T
  rtu_alpha_target, real_T *rty_V_alpha_target)
{
  *rty_V_alpha_target = std::sqrt(std::abs(rtu_alpha - rtu_alpha_0) / (rtu_alpha_target - rtu_alpha_0)) * rtu_v_ias;
}

void FlyByWireModelClass::step()
{
  real_T rtb_BusAssignment_a_sim_data_pk_deg_s;
  real_T rtb_BusAssignment_a_sim_data_qk_dot_deg_s2;
  real_T rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_floor_deg;
  real_T rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_max_deg;
  real_T rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg;
  real_T rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_lo;
  real_T rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_up;
  real_T rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain;
  real_T rtb_BusAssignment_cs_sim_data_qk_deg_s;
  real_T rtb_BusAssignment_e_roll_law_normal_xi_deg;
  real_T rtb_BusAssignment_f_pitch_law_normal_eta_dot_deg_s;
  real_T rtb_BusAssignment_m_pitch_output_eta_deg;
  real_T rtb_BusAssignment_p_roll_data_computed_delta_xi_deg;
  real_T rtb_BusAssignment_p_roll_data_computed_in_flight_gain;
  real_T rtb_BusAssignment_sim_data_zeta_trim_deg;
  real_T rtb_BusAssignment_sim_input_delta_eta_pos;
  real_T rtb_BusAssignment_sim_input_delta_xi_pos;
  real_T rtb_BusAssignment_sim_input_delta_zeta_pos;
  real_T rtb_Divide1;
  real_T rtb_Divide_b;
  real_T rtb_Divide_e;
  real_T rtb_Divide_m;
  real_T rtb_Gain;
  real_T rtb_Gain1_mp;
  real_T rtb_GainPhi;
  real_T rtb_GainTheta;
  real_T rtb_Gain_c;
  real_T rtb_Gain_g;
  real_T rtb_Gain_kh;
  real_T rtb_Gainpk;
  real_T rtb_Gainpk2;
  real_T rtb_Gainpk4;
  real_T rtb_Gainqk;
  real_T rtb_LimiteriH;
  real_T rtb_Limiterxi;
  real_T rtb_Limiterxi1;
  real_T rtb_Limiterxi2;
  real_T rtb_ManualSwitch;
  real_T rtb_Min5;
  real_T rtb_Product1_n5;
  real_T rtb_Product_e;
  real_T rtb_Sum1_a;
  real_T rtb_Sum_l;
  real_T rtb_Switch;
  real_T rtb_Y;
  real_T rtb_Y_dd;
  real_T rtb_Y_e;
  real_T rtb_Y_h;
  real_T rtb_Y_k;
  real_T rtb_Y_o;
  real_T rtb_eta_trim_deg_rate_limit_lo_deg_s;
  real_T rtb_nz_limit_up_g;
  real_T rtb_v_target;
  real_T rtb_y_g;
  real_T u0;
  real_T u0_0;
  real_T u0_1;
  real_T u1;
  int32_T rtb_alpha_floor_inhib;
  int32_T rtb_ap_special_disc;
  int32_T rtb_in_flare;
  int32_T rtb_in_rotation;
  int32_T rtb_nz_limit_lo_g;
  int32_T rtb_on_ground;
  boolean_T guard1 = false;
  boolean_T rtb_AND;
  boolean_T rtb_NOT2;
  boolean_T rtb_eta_trim_deg_reset;
  boolean_T rtb_eta_trim_deg_should_freeze;
  boolean_T rtb_eta_trim_deg_should_write;
  FlyByWire_DWork.Delay_DSTATE += FlyByWire_U.in.time.dt;
  rtb_GainTheta = FlyByWire_P.GainTheta_Gain * FlyByWire_U.in.data.Theta_deg;
  rtb_GainPhi = FlyByWire_P.GainPhi_Gain * FlyByWire_U.in.data.Phi_deg;
  rtb_Gainqk = FlyByWire_P.Gain_Gain_n * FlyByWire_U.in.data.q_rad_s * FlyByWire_P.Gainqk_Gain;
  rtb_Gain = FlyByWire_P.Gain_Gain_l * FlyByWire_U.in.data.r_rad_s;
  rtb_Gainpk = FlyByWire_P.Gain_Gain_a * FlyByWire_U.in.data.p_rad_s * FlyByWire_P.Gainpk_Gain;
  FlyByWire_ConvertToEuler(rtb_GainTheta, rtb_GainPhi, rtb_Gainqk, rtb_Gain, rtb_Gainpk, &rtb_Y, &rtb_Y_k, &rtb_Y_h);
  FlyByWire_ConvertToEuler(rtb_GainTheta, rtb_GainPhi, FlyByWire_P.Gainqk1_Gain * (FlyByWire_P.Gain_Gain_e *
    FlyByWire_U.in.data.q_dot_rad_s2), FlyByWire_P.Gain_Gain_aw * FlyByWire_U.in.data.r_dot_rad_s2,
    FlyByWire_P.Gainpk1_Gain * (FlyByWire_P.Gain_Gain_nm * FlyByWire_U.in.data.p_dot_rad_s2), &rtb_Y_e, &rtb_Y_o,
    &rtb_Y_dd);
  rtb_Gainpk4 = FlyByWire_P.Gainpk4_Gain * FlyByWire_U.in.data.eta_pos;
  rtb_Gainpk2 = FlyByWire_P.Gainpk2_Gain * FlyByWire_U.in.data.eta_trim_deg;
  rtb_Sum_l = FlyByWire_P.Gainpk5_Gain * FlyByWire_U.in.data.xi_pos;
  rtb_Limiterxi2 = FlyByWire_P.Gainpk6_Gain * FlyByWire_U.in.data.zeta_pos;
  rtb_Limiterxi1 = FlyByWire_P.Gainpk3_Gain * FlyByWire_U.in.data.zeta_trim_pos;
  u0 = FlyByWire_P.Gain1_Gain_h * FlyByWire_U.in.data.gear_animation_pos_1 - FlyByWire_P.Constant_Value_g;
  if (u0 > FlyByWire_P.Saturation1_UpperSat_g) {
    u0 = FlyByWire_P.Saturation1_UpperSat_g;
  } else {
    if (u0 < FlyByWire_P.Saturation1_LowerSat_j) {
      u0 = FlyByWire_P.Saturation1_LowerSat_j;
    }
  }

  u0_0 = FlyByWire_P.Gain2_Gain_a * FlyByWire_U.in.data.gear_animation_pos_2 - FlyByWire_P.Constant_Value_g;
  if (u0_0 > FlyByWire_P.Saturation2_UpperSat_b) {
    u0_0 = FlyByWire_P.Saturation2_UpperSat_b;
  } else {
    if (u0_0 < FlyByWire_P.Saturation2_LowerSat_g) {
      u0_0 = FlyByWire_P.Saturation2_LowerSat_g;
    }
  }

  rtb_Sum1_a = FlyByWire_P.Gaineta_Gain * FlyByWire_U.in.input.delta_eta_pos;
  rtb_LimiteriH = FlyByWire_P.Gainxi_Gain * FlyByWire_U.in.input.delta_xi_pos;
  rtb_Limiterxi = FlyByWire_P.Gainxi1_Gain * FlyByWire_U.in.input.delta_zeta_pos;
  FlyByWire_Y.out.sim.data.xi_deg = rtb_Sum_l;
  FlyByWire_Y.out.sim.data.zeta_deg = rtb_Limiterxi2;
  rtb_BusAssignment_sim_data_zeta_trim_deg = rtb_Limiterxi1;
  rtb_BusAssignment_sim_input_delta_eta_pos = rtb_Sum1_a;
  rtb_BusAssignment_sim_input_delta_xi_pos = rtb_LimiteriH;
  rtb_BusAssignment_sim_input_delta_zeta_pos = rtb_Limiterxi;
  FlyByWire_LagFilter(FlyByWire_U.in.data.alpha_deg, FlyByWire_P.LagFilter_C1, FlyByWire_U.in.time.dt, &rtb_Limiterxi2,
                      &FlyByWire_DWork.sf_LagFilter_n);
  FlyByWire_RateLimiter(look2_binlxpw(FlyByWire_U.in.data.V_mach, FlyByWire_U.in.data.flaps_handle_index,
    FlyByWire_P.alphamax_bp01Data, FlyByWire_P.alphamax_bp02Data, FlyByWire_P.alphamax_tableData,
    FlyByWire_P.alphamax_maxIndex, 4U), FlyByWire_P.RateLimiterVariableTs2_up, FlyByWire_P.RateLimiterVariableTs2_lo,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs2_InitialCondition, &rtb_Limiterxi,
                        &FlyByWire_DWork.sf_RateLimiter_pr);
  FlyByWire_RateLimiter(look1_binlxpw(FlyByWire_U.in.data.flaps_handle_index, FlyByWire_P.alpha0_bp01Data,
    FlyByWire_P.alpha0_tableData, 5U), FlyByWire_P.RateLimiterVariableTs3_up, FlyByWire_P.RateLimiterVariableTs3_lo,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs3_InitialCondition, &rtb_LimiteriH,
                        &FlyByWire_DWork.sf_RateLimiter_b5);
  FlyByWire_CalculateV_alpha_max(FlyByWire_U.in.data.V_ias_kn, rtb_Limiterxi2, rtb_LimiteriH, rtb_Limiterxi, &rtb_Sum_l);
  FlyByWire_TimeSinceCondition(FlyByWire_U.in.time.simulation_time,
    FlyByWire_P.fbw_output_MATLABStruct.sim.data_computed.on_ground == 0.0, &rtb_Sum1_a,
    &FlyByWire_DWork.sf_TimeSinceCondition_m);
  FlyByWire_RateLimiter(look2_binlxpw(FlyByWire_U.in.data.V_mach, FlyByWire_U.in.data.flaps_handle_index,
    FlyByWire_P.alphaprotection_bp01Data, FlyByWire_P.alphaprotection_bp02Data, FlyByWire_P.alphaprotection_tableData,
    FlyByWire_P.alphaprotection_maxIndex, 4U), FlyByWire_P.RateLimiterVariableTs_up,
                        FlyByWire_P.RateLimiterVariableTs_lo, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs_InitialCondition, &rtb_Limiterxi1,
                        &FlyByWire_DWork.sf_RateLimiter_e);
  if (rtb_Sum1_a <= FlyByWire_P.CompareToConstant_const) {
    rtb_Switch = rtb_Limiterxi;
  } else {
    rtb_Switch = rtb_Limiterxi1;
  }

  FlyByWire_CalculateV_alpha_max(FlyByWire_U.in.data.V_ias_kn, rtb_Limiterxi2, rtb_LimiteriH, rtb_Switch,
    &rtb_Limiterxi1);
  FlyByWire_RateLimiter(look2_binlxpw(FlyByWire_U.in.data.V_mach, FlyByWire_U.in.data.flaps_handle_index,
    FlyByWire_P.alphafloor_bp01Data, FlyByWire_P.alphafloor_bp02Data, FlyByWire_P.alphafloor_tableData,
    FlyByWire_P.alphafloor_maxIndex, 4U), FlyByWire_P.RateLimiterVariableTs1_up, FlyByWire_P.RateLimiterVariableTs1_lo,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs1_InitialCondition, &rtb_Sum1_a,
                        &FlyByWire_DWork.sf_RateLimiter_bu);
  FlyByWire_Y.out.sim.data.pk_dot_deg_s2 = rtb_Y_dd;
  FlyByWire_Y.out.sim.data_speeds_aoa.v_alpha_max_kn = rtb_Sum_l;
  rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_max_deg = rtb_Limiterxi;
  FlyByWire_Y.out.sim.data_speeds_aoa.v_alpha_prot_kn = rtb_Limiterxi1;
  rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_floor_deg = rtb_Sum1_a;
  FlyByWire_Y.out.sim.data_speeds_aoa.alpha_filtered_deg = rtb_Limiterxi2;
  rtb_Sum1_a = rtb_GainTheta - std::cos(FlyByWire_P.Gain1_Gain_c * rtb_GainPhi) * FlyByWire_U.in.data.alpha_deg;
  FlyByWire_GetIASforMach3(FlyByWire_U.in.data.V_mach, look1_binlxpw(rtb_Sum1_a, FlyByWire_P.uDLookupTable2_bp01Data,
    FlyByWire_P.uDLookupTable2_tableData, 3U), FlyByWire_U.in.data.V_ias_kn, &rtb_Y_dd);
  u0_1 = look1_binlxpw(rtb_Sum1_a, FlyByWire_P.uDLookupTable1_bp01Data, FlyByWire_P.uDLookupTable1_tableData, 3U);
  if (u0_1 >= rtb_Y_dd) {
    u0_1 = rtb_Y_dd;
  }

  FlyByWire_GetIASforMach4(FlyByWire_U.in.data.V_mach, FlyByWire_P.Constant6_Value, FlyByWire_U.in.data.V_ias_kn,
    &rtb_Y_dd);
  if (FlyByWire_P.Constant5_Value < rtb_Y_dd) {
    rtb_LimiteriH = FlyByWire_P.Constant5_Value;
  } else {
    rtb_LimiteriH = rtb_Y_dd;
  }

  FlyByWire_DWork.Memory_PreviousInput = FlyByWire_P.Logic_table[(((static_cast<uint32_T>(FlyByWire_U.in.data.V_ias_kn >
    u0_1) << 1) + (FlyByWire_U.in.data.V_ias_kn < rtb_LimiteriH)) << 1) + FlyByWire_DWork.Memory_PreviousInput];
  rtb_Limiterxi = ((FlyByWire_U.in.data.autopilot_custom_on == 0.0) && FlyByWire_DWork.Memory_PreviousInput);
  if (FlyByWire_DWork.is_active_c1_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c1_FlyByWire = 1U;
    FlyByWire_DWork.is_c1_FlyByWire = FlyByWire_IN_OnGround;
    rtb_on_ground = 1;
  } else if (FlyByWire_DWork.is_c1_FlyByWire == 1) {
    if ((u0 > 0.1) || (u0_0 > 0.1)) {
      FlyByWire_DWork.is_c1_FlyByWire = FlyByWire_IN_OnGround;
      rtb_on_ground = 1;
    } else {
      rtb_on_ground = 0;
    }
  } else {
    if ((u0 == 0.0) && (u0_0 == 0.0)) {
      FlyByWire_DWork.is_c1_FlyByWire = FlyByWire_IN_InAir;
      rtb_on_ground = 0;
    } else {
      rtb_on_ground = 1;
    }
  }

  rtb_Limiterxi1 = ((FlyByWire_U.in.data.autopilot_master_on != 0.0) || (FlyByWire_U.in.data.slew_on != 0.0) ||
                    (FlyByWire_U.in.data.pause_on != 0.0) || (FlyByWire_U.in.data.tracking_mode_on_override != 0.0));
  if (!FlyByWire_DWork.resetEventTime_not_empty) {
    FlyByWire_DWork.resetEventTime = FlyByWire_U.in.time.simulation_time;
    FlyByWire_DWork.resetEventTime_not_empty = true;
  }

  if ((rtb_BusAssignment_sim_input_delta_eta_pos >= -0.03125) || (rtb_Limiterxi2 >=
       rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_max_deg) || (FlyByWire_DWork.resetEventTime == 0.0)) {
    FlyByWire_DWork.resetEventTime = FlyByWire_U.in.time.simulation_time;
  }

  if ((rtb_on_ground == 0) && (rtb_Limiterxi2 > rtb_Switch)) {
    FlyByWire_DWork.sProtActive = 1.0;
  }

  if ((FlyByWire_U.in.time.simulation_time - FlyByWire_DWork.resetEventTime > 0.5) ||
      (rtb_BusAssignment_sim_input_delta_eta_pos < -0.5) || ((FlyByWire_U.in.data.H_radio_ft < 200.0) &&
       (rtb_BusAssignment_sim_input_delta_eta_pos < 0.5) && (rtb_Limiterxi2 < rtb_Switch - 2.0)) || (rtb_on_ground != 0))
  {
    FlyByWire_DWork.sProtActive = 0.0;
  }

  rtb_AND = ((FlyByWire_DWork.sProtActive != 0.0) && (FlyByWire_U.in.data.autopilot_custom_on == 0.0));
  rtb_Gain_kh = FlyByWire_P.DiscreteDerivativeVariableTs_Gain * FlyByWire_U.in.data.V_ias_kn;
  FlyByWire_LagFilter((rtb_Gain_kh - FlyByWire_DWork.Delay_DSTATE_d) / FlyByWire_U.in.time.dt,
                      FlyByWire_P.LagFilter_C1_a, FlyByWire_U.in.time.dt, &rtb_Y_dd, &FlyByWire_DWork.sf_LagFilter);
  if (FlyByWire_DWork.is_active_c15_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c15_FlyByWire = 1U;
    FlyByWire_DWork.is_c15_FlyByWire = FlyByWire_IN_Landed;
    rtb_alpha_floor_inhib = 1;
    rtb_ap_special_disc = 0;
  } else {
    switch (FlyByWire_DWork.is_c15_FlyByWire) {
     case FlyByWire_IN_Flying:
      if (FlyByWire_U.in.data.H_radio_ft < 100.0) {
        FlyByWire_DWork.is_c15_FlyByWire = FlyByWire_IN_Landing100ft;
        rtb_alpha_floor_inhib = 1;
        rtb_ap_special_disc = 1;
      } else {
        rtb_alpha_floor_inhib = 0;
        rtb_ap_special_disc = 0;
      }
      break;

     case FlyByWire_IN_Landed:
      if (rtb_on_ground == 0) {
        FlyByWire_DWork.is_c15_FlyByWire = FlyByWire_IN_Takeoff100ft;
        rtb_alpha_floor_inhib = 0;
        rtb_ap_special_disc = 0;
      } else {
        rtb_alpha_floor_inhib = 1;
        rtb_ap_special_disc = 0;
      }
      break;

     case FlyByWire_IN_Landing100ft:
      if (rtb_on_ground != 0) {
        FlyByWire_DWork.is_c15_FlyByWire = FlyByWire_IN_Landed;
        rtb_alpha_floor_inhib = 1;
        rtb_ap_special_disc = 0;
      } else if (FlyByWire_U.in.data.H_radio_ft > 100.0) {
        FlyByWire_DWork.is_c15_FlyByWire = FlyByWire_IN_Flying;
        rtb_alpha_floor_inhib = 0;
        rtb_ap_special_disc = 0;
      } else {
        rtb_alpha_floor_inhib = 1;
        rtb_ap_special_disc = 1;
      }
      break;

     default:
      if (FlyByWire_U.in.data.H_radio_ft > 100.0) {
        FlyByWire_DWork.is_c15_FlyByWire = FlyByWire_IN_Flying;
        rtb_alpha_floor_inhib = 0;
        rtb_ap_special_disc = 0;
      } else {
        rtb_alpha_floor_inhib = 0;
        rtb_ap_special_disc = 0;
      }
      break;
    }
  }

  guard1 = false;
  if ((rtb_alpha_floor_inhib == 0) && (FlyByWire_U.in.data.V_mach < 0.6)) {
    if (FlyByWire_U.in.data.flaps_handle_index >= 4.0) {
      rtb_nz_limit_lo_g = -3;
    } else {
      rtb_nz_limit_lo_g = 0;
    }

    if (rtb_Y_dd <= rtb_nz_limit_lo_g) {
      rtb_Y_dd = rtb_nz_limit_lo_g;
    }

    if (rtb_Y_dd >= 0.0) {
      rtb_Y_dd = 0.0;
    }

    if (rtb_Limiterxi2 > rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_floor_deg + rtb_Y_dd) {
      FlyByWire_DWork.sAlphaFloor = 1.0;
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    if ((rtb_alpha_floor_inhib != 0) || (FlyByWire_DWork.sProtActive == 0.0)) {
      FlyByWire_DWork.sAlphaFloor = 0.0;
    }
  }

  rtb_Sum1_a = look1_binlxpw(rtb_Sum1_a, FlyByWire_P.uDLookupTable_bp01Data, FlyByWire_P.uDLookupTable_tableData, 3U);
  FlyByWire_GetIASforMach3(FlyByWire_U.in.data.V_mach, rtb_Sum1_a + FlyByWire_P.Bias_Bias, FlyByWire_U.in.data.V_ias_kn,
    &rtb_Y_dd);
  if (rtb_Y_dd < FlyByWire_P.Constant_Value_i) {
    rtb_Sum1_a = rtb_Y_dd;
  } else {
    rtb_Sum1_a = FlyByWire_P.Constant_Value_i;
  }

  FlyByWire_TimeSinceCondition(FlyByWire_U.in.time.simulation_time, rtb_Sum1_a < FlyByWire_U.in.data.V_ias_kn, &rtb_Y_dd,
    &FlyByWire_DWork.sf_TimeSinceCondition);
  rtb_NOT2 = (((rtb_on_ground == 0) && (((rtb_ap_special_disc != 0) && (rtb_Limiterxi2 >
    rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_max_deg)) || (rtb_Limiterxi2 > rtb_Switch + 0.25))) || (rtb_Y_dd >
    FlyByWire_P.CompareToConstant_const_h));
  FlyByWire_GetIASforMach4(FlyByWire_U.in.data.V_mach, FlyByWire_P.Constant8_Value, FlyByWire_U.in.data.V_ias_kn,
    &rtb_Y_dd);
  if (FlyByWire_P.Constant7_Value < rtb_Y_dd) {
    rtb_Min5 = FlyByWire_P.Constant7_Value;
  } else {
    rtb_Min5 = rtb_Y_dd;
  }

  FlyByWire_Y.out.sim.data.qk_deg_s = rtb_Y;
  FlyByWire_Y.out.sim.data.rk_deg_s = rtb_Y_k;
  rtb_BusAssignment_a_sim_data_pk_deg_s = rtb_Y_h;
  rtb_BusAssignment_a_sim_data_qk_dot_deg_s2 = rtb_Y_e;
  FlyByWire_Y.out.sim.data.rk_dot_deg_s2 = rtb_Y_o;
  rtb_alpha_floor_inhib = static_cast<int32_T>(rtb_Limiterxi1);
  FlyByWire_Y.out.sim.data_computed.high_aoa_prot_active = rtb_AND;
  rtb_ap_special_disc = static_cast<int32_T>(rtb_Limiterxi);
  FlyByWire_Y.out.sim.data_computed.high_speed_prot_low_kn = rtb_LimiteriH;
  FlyByWire_eta_trim_limit_lofreeze(rtb_Gainpk2, static_cast<real_T>(rtb_AND), &rtb_Y_dd,
    &FlyByWire_DWork.sf_eta_trim_limit_lofreeze);
  if (static_cast<real_T>(rtb_AND) > FlyByWire_P.Switch_Threshold_h) {
    rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_lo = rtb_Y_dd;
  } else {
    rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_lo = FlyByWire_P.Constant3_Value;
  }

  FlyByWire_eta_trim_limit_lofreeze(rtb_Gainpk2, rtb_Limiterxi, &rtb_Y_dd, &FlyByWire_DWork.sf_eta_trim_limit_upfreeze);
  if (rtb_Limiterxi > FlyByWire_P.Switch1_Threshold_k) {
    rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_up = rtb_Y_dd;
  } else {
    rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_up = FlyByWire_P.Constant2_Value;
  }

  if (FlyByWire_DWork.is_active_c3_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c3_FlyByWire = 1U;
    FlyByWire_DWork.is_c3_FlyByWire = FlyByWire_IN_Ground_p;
    FlyByWire_B.in_flight = 0.0;
  } else {
    switch (FlyByWire_DWork.is_c3_FlyByWire) {
     case FlyByWire_IN_Flight:
      if ((rtb_on_ground == 1) && (rtb_GainTheta < 2.5)) {
        FlyByWire_DWork.on_ground_time = FlyByWire_U.in.time.simulation_time;
        FlyByWire_DWork.is_c3_FlyByWire = FlyByWire_IN_FlightToGroundTransition;
      } else {
        FlyByWire_B.in_flight = 1.0;
      }
      break;

     case FlyByWire_IN_FlightToGroundTransition:
      if (FlyByWire_U.in.time.simulation_time - FlyByWire_DWork.on_ground_time >= 5.0) {
        FlyByWire_DWork.is_c3_FlyByWire = FlyByWire_IN_Ground_p;
        FlyByWire_B.in_flight = 0.0;
      } else {
        if ((rtb_on_ground == 0) || (rtb_GainTheta >= 2.5)) {
          FlyByWire_DWork.on_ground_time = 0.0;
          FlyByWire_DWork.is_c3_FlyByWire = FlyByWire_IN_Flight;
          FlyByWire_B.in_flight = 1.0;
        }
      }
      break;

     default:
      if (((rtb_on_ground == 0) && (rtb_GainTheta > 8.0)) || (FlyByWire_U.in.data.H_radio_ft > 400.0)) {
        FlyByWire_DWork.on_ground_time = 0.0;
        FlyByWire_DWork.is_c3_FlyByWire = FlyByWire_IN_Flight;
        FlyByWire_B.in_flight = 1.0;
      } else {
        FlyByWire_B.in_flight = 0.0;
      }
      break;
    }
  }

  if (FlyByWire_B.in_flight > FlyByWire_P.Saturation_UpperSat_er) {
    u0_1 = FlyByWire_P.Saturation_UpperSat_er;
  } else if (FlyByWire_B.in_flight < FlyByWire_P.Saturation_LowerSat_a) {
    u0_1 = FlyByWire_P.Saturation_LowerSat_a;
  } else {
    u0_1 = FlyByWire_B.in_flight;
  }

  FlyByWire_RateLimiter(u0_1, FlyByWire_P.RateLimiterVariableTs_up_d, FlyByWire_P.RateLimiterVariableTs_lo_c,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs_InitialCondition_d, &rtb_Y_h,
                        &FlyByWire_DWork.sf_RateLimiter_b);
  if (FlyByWire_DWork.is_active_c6_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c6_FlyByWire = 1U;
    FlyByWire_DWork.is_c6_FlyByWire = FlyByWire_IN_OFF;
    rtb_in_rotation = 0;
  } else if (FlyByWire_DWork.is_c6_FlyByWire == 1) {
    if ((rtb_Y_h < 1.0) && (FlyByWire_U.in.data.V_tas_kn > 70.0) && ((FlyByWire_U.in.data.thrust_lever_1_pos >= 35.0) ||
         (FlyByWire_U.in.data.thrust_lever_2_pos >= 35.0))) {
      FlyByWire_DWork.is_c6_FlyByWire = FlyByWire_IN_ON;
      rtb_in_rotation = 1;
    } else {
      rtb_in_rotation = 0;
    }
  } else {
    if ((rtb_Y_h == 1.0) || (FlyByWire_U.in.data.H_radio_ft > 400.0) || ((FlyByWire_U.in.data.V_tas_kn < 70.0) &&
         ((FlyByWire_U.in.data.thrust_lever_1_pos < 35.0) || (FlyByWire_U.in.data.thrust_lever_2_pos < 35.0)))) {
      FlyByWire_DWork.is_c6_FlyByWire = FlyByWire_IN_OFF;
      rtb_in_rotation = 0;
    } else {
      rtb_in_rotation = 1;
    }
  }

  FlyByWire_LagFilter(rtb_GainTheta, FlyByWire_P.LagFilter_C1_n, FlyByWire_U.in.time.dt, &rtb_Y_k,
                      &FlyByWire_DWork.sf_LagFilter_l);
  if (FlyByWire_P.ManualSwitch_CurrentSetting == 1) {
    rtb_ManualSwitch = FlyByWire_P.Constant1_Value_f;
  } else {
    rtb_ManualSwitch = FlyByWire_P.Constant_Value_jz;
  }

  if (FlyByWire_DWork.is_active_c2_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c2_FlyByWire = 1U;
    FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Ground;
    rtb_in_flare = 0;
    FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
    FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
  } else {
    switch (FlyByWire_DWork.is_c2_FlyByWire) {
     case FlyByWire_IN_Flare_Reduce_Theta_c:
      if (FlyByWire_B.in_flight == 0.0) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Ground;
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      } else if ((FlyByWire_U.in.data.H_radio_ft > 50.0) && (rtb_ManualSwitch == 0.0)) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flight_Low;
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      } else {
        rtb_in_flare = 1;
        FlyByWire_B.flare_Theta_c_deg = -2.0;
      }
      break;

     case FlyByWire_IN_Flare_Set_Rate:
      if (FlyByWire_P.ManualSwitch1_CurrentSetting == 1) {
        u0_1 = FlyByWire_P.Constant1_Value_f;
      } else {
        u0_1 = FlyByWire_P.Constant_Value_jz;
      }

      if ((FlyByWire_U.in.data.H_radio_ft <= 30.0) || (u0_1 == 1.0)) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flare_Reduce_Theta_c;
        rtb_in_flare = 1;
        FlyByWire_B.flare_Theta_c_deg = -2.0;
      } else if ((FlyByWire_U.in.data.H_radio_ft > 50.0) && (rtb_ManualSwitch == 0.0)) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flight_Low;
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      } else {
        rtb_in_flare = 1;
      }
      break;

     case FlyByWire_IN_Flare_Store_Theta_c_deg:
      if ((FlyByWire_U.in.data.H_radio_ft > 50.0) && (rtb_ManualSwitch == 0.0)) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flight_Low;
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      } else {
        FlyByWire_B.flare_Theta_c_rate_deg_s = -(rtb_Y_k + 2.0) / 8.0;
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flare_Set_Rate;
        rtb_in_flare = 1;
      }
      break;

     case FlyByWire_IN_Flight_High:
      if ((FlyByWire_U.in.data.H_radio_ft <= 50.0) || (rtb_ManualSwitch == 1.0)) {
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flare_Store_Theta_c_deg;
        rtb_in_flare = 1;
      } else {
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      }
      break;

     case FlyByWire_IN_Flight_Low:
      if (FlyByWire_U.in.data.H_radio_ft > 50.0) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flight_High;
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      } else {
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      }
      break;

     default:
      if (FlyByWire_B.in_flight == 1.0) {
        FlyByWire_DWork.is_c2_FlyByWire = FlyByWire_IN_Flight_Low;
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      } else {
        rtb_in_flare = 0;
        FlyByWire_B.flare_Theta_c_deg = rtb_Y_k;
        FlyByWire_B.flare_Theta_c_rate_deg_s = -3.0;
      }
      break;
    }
  }

  if (rtb_in_rotation > FlyByWire_P.Saturation1_UpperSat_f) {
    rtb_ManualSwitch = FlyByWire_P.Saturation1_UpperSat_f;
  } else if (rtb_in_rotation < FlyByWire_P.Saturation1_LowerSat_p) {
    rtb_ManualSwitch = FlyByWire_P.Saturation1_LowerSat_p;
  } else {
    rtb_ManualSwitch = rtb_in_rotation;
  }

  FlyByWire_RateLimiter(rtb_ManualSwitch, FlyByWire_P.RateLimiterVariableTs1_up_n,
                        FlyByWire_P.RateLimiterVariableTs1_lo_c, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs1_InitialCondition_h, &rtb_Y_e,
                        &FlyByWire_DWork.sf_RateLimiter_g);
  if (FlyByWire_DWork.is_active_c7_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c7_FlyByWire = 1U;
    FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_ground;
    rtb_ManualSwitch = 0.7;
    rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
    rtb_nz_limit_up_g = 2.0;
    rtb_nz_limit_lo_g = 0;
  } else {
    switch (FlyByWire_DWork.is_c7_FlyByWire) {
     case FlyByWire_IN_flight_clean:
      if (FlyByWire_U.in.data.flaps_handle_index != 0.0) {
        FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_flight_flaps;
        rtb_ManualSwitch = 0.7;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
        rtb_nz_limit_up_g = 2.0;
        rtb_nz_limit_lo_g = 0;
      } else if ((FlyByWire_B.in_flight == 0.0) && (FlyByWire_U.in.data.flaps_handle_index == 0.0)) {
        FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_ground;
        rtb_ManualSwitch = 0.7;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
        rtb_nz_limit_up_g = 2.0;
        rtb_nz_limit_lo_g = 0;
      } else {
        rtb_ManualSwitch = 0.3;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.3;
        rtb_nz_limit_up_g = 2.5;
        rtb_nz_limit_lo_g = -1;
      }
      break;

     case FlyByWire_IN_flight_flaps:
      if (FlyByWire_U.in.data.flaps_handle_index == 0.0) {
        FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_flight_clean;
        rtb_ManualSwitch = 0.3;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.3;
        rtb_nz_limit_up_g = 2.5;
        rtb_nz_limit_lo_g = -1;
      } else if (FlyByWire_B.in_flight == 0.0) {
        FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_ground;
        rtb_ManualSwitch = 0.7;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
        rtb_nz_limit_up_g = 2.0;
        rtb_nz_limit_lo_g = 0;
      } else {
        rtb_ManualSwitch = 0.7;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
        rtb_nz_limit_up_g = 2.0;
        rtb_nz_limit_lo_g = 0;
      }
      break;

     default:
      if ((FlyByWire_B.in_flight != 0.0) && (FlyByWire_U.in.data.flaps_handle_index == 0.0)) {
        FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_flight_clean;
        rtb_ManualSwitch = 0.3;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.3;
        rtb_nz_limit_up_g = 2.5;
        rtb_nz_limit_lo_g = -1;
      } else if ((FlyByWire_B.in_flight != 0.0) && (FlyByWire_U.in.data.flaps_handle_index != 0.0)) {
        FlyByWire_DWork.is_c7_FlyByWire = FlyByWire_IN_flight_flaps;
        rtb_ManualSwitch = 0.7;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
        rtb_nz_limit_up_g = 2.0;
        rtb_nz_limit_lo_g = 0;
      } else {
        rtb_ManualSwitch = 0.7;
        rtb_eta_trim_deg_rate_limit_lo_deg_s = -0.7;
        rtb_nz_limit_up_g = 2.0;
        rtb_nz_limit_lo_g = 0;
      }
      break;
    }
  }

  FlyByWire_RateLimiter(rtb_nz_limit_up_g, FlyByWire_P.RateLimiterVariableTs2_up_f,
                        FlyByWire_P.RateLimiterVariableTs2_lo_m, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs2_InitialCondition_b, &rtb_Y_o,
                        &FlyByWire_DWork.sf_RateLimiter_m);
  FlyByWire_RateLimiter(static_cast<real_T>(rtb_nz_limit_lo_g), FlyByWire_P.RateLimiterVariableTs3_up_c,
                        FlyByWire_P.RateLimiterVariableTs3_lo_l, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs3_InitialCondition_b, &rtb_Y_dd,
                        &FlyByWire_DWork.sf_RateLimiter_a);
  if (FlyByWire_DWork.is_active_c9_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c9_FlyByWire = 1U;
    FlyByWire_DWork.is_c9_FlyByWire = FlyByWire_IN_running;
    rtb_eta_trim_deg_should_freeze = false;
  } else if (FlyByWire_DWork.is_c9_FlyByWire == 1) {
    if ((rtb_in_flare == 0) && (FlyByWire_U.in.data.nz_g < 1.25) && (FlyByWire_U.in.data.nz_g > 0.5) && (std::abs
         (rtb_GainPhi) <= 30.0)) {
      FlyByWire_DWork.is_c9_FlyByWire = FlyByWire_IN_running;
      rtb_eta_trim_deg_should_freeze = false;
    } else {
      rtb_eta_trim_deg_should_freeze = true;
    }
  } else {
    if ((rtb_in_flare != 0) || (FlyByWire_U.in.data.nz_g >= 1.25) || (FlyByWire_U.in.data.nz_g <= 0.5) || (std::abs
         (rtb_GainPhi) > 30.0)) {
      FlyByWire_DWork.is_c9_FlyByWire = FlyByWire_IN_frozen;
      rtb_eta_trim_deg_should_freeze = true;
    } else {
      rtb_eta_trim_deg_should_freeze = false;
    }
  }

  if (FlyByWire_DWork.is_active_c8_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c8_FlyByWire = 1U;
    FlyByWire_DWork.is_c8_FlyByWire = FlyByWire_IN_manual;
    rtb_eta_trim_deg_reset = true;
    rtb_nz_limit_up_g = rtb_Gainpk2;
    rtb_eta_trim_deg_should_write = false;
  } else {
    switch (FlyByWire_DWork.is_c8_FlyByWire) {
     case FlyByWire_IN_automatic:
      if (FlyByWire_B.in_flight == 0.0) {
        FlyByWire_DWork.is_c8_FlyByWire = FlyByWire_IN_reset;
        rtb_eta_trim_deg_reset = true;
        rtb_nz_limit_up_g = 0.0;
        rtb_eta_trim_deg_should_write = true;
      } else if (rtb_Limiterxi1 != 0.0) {
        FlyByWire_DWork.is_c8_FlyByWire = FlyByWire_IN_tracking;
        rtb_eta_trim_deg_reset = true;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = false;
      } else {
        rtb_eta_trim_deg_reset = false;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = true;
      }
      break;

     case FlyByWire_IN_manual:
      if (FlyByWire_B.in_flight != 0.0) {
        FlyByWire_DWork.is_c8_FlyByWire = FlyByWire_IN_automatic;
        rtb_eta_trim_deg_reset = false;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = true;
      } else {
        rtb_eta_trim_deg_reset = true;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = false;
      }
      break;

     case FlyByWire_IN_reset:
      if ((FlyByWire_B.in_flight == 0.0) && (rtb_Gainpk2 == 0.0)) {
        FlyByWire_DWork.is_c8_FlyByWire = FlyByWire_IN_manual;
        rtb_eta_trim_deg_reset = true;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = false;
      } else {
        rtb_eta_trim_deg_reset = true;
        rtb_nz_limit_up_g = 0.0;
        rtb_eta_trim_deg_should_write = true;
      }
      break;

     default:
      if (rtb_Limiterxi1 == 0.0) {
        FlyByWire_DWork.is_c8_FlyByWire = FlyByWire_IN_automatic;
        rtb_eta_trim_deg_reset = false;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = true;
      } else {
        rtb_eta_trim_deg_reset = true;
        rtb_nz_limit_up_g = rtb_Gainpk2;
        rtb_eta_trim_deg_should_write = false;
      }
      break;
    }
  }

  u0_1 = FlyByWire_B.flare_Theta_c_deg - FlyByWire_DWork.Delay_DSTATE_dq;
  u1 = std::abs(FlyByWire_B.flare_Theta_c_rate_deg_s) * FlyByWire_U.in.time.dt;
  if (u0_1 < u1) {
    u1 = u0_1;
  }

  rtb_Limiterxi1 = FlyByWire_U.in.time.dt * FlyByWire_B.flare_Theta_c_rate_deg_s;
  if (u1 > rtb_Limiterxi1) {
    rtb_Limiterxi1 = u1;
  }

  FlyByWire_DWork.Delay_DSTATE_dq += rtb_Limiterxi1;
  rtb_BusAssignment_cs_sim_data_qk_deg_s = rtb_Y;
  rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg = FlyByWire_P.Gain_Gain_d *
    rtb_BusAssignment_sim_input_delta_eta_pos;
  rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain = rtb_Y_h;
  FlyByWire_Y.out.pitch.data_computed.nz_limit_lo_g = rtb_Y_dd;
  FlyByWire_Y.out.pitch.data_computed.flare_Theta_deg = rtb_Y_k;
  rtb_Sum1_a = std::cos(FlyByWire_P.Gain1_Gain_p * rtb_GainTheta);
  rtb_Divide1 = rtb_Sum1_a / std::cos(FlyByWire_P.Gain1_Gain_pa * rtb_GainPhi);
  if (rtb_GainPhi > FlyByWire_P.Saturation_UpperSat_d) {
    rtb_Gain_g = FlyByWire_P.Saturation_UpperSat_d;
  } else if (rtb_GainPhi < FlyByWire_P.Saturation_LowerSat_pr) {
    rtb_Gain_g = FlyByWire_P.Saturation_LowerSat_pr;
  } else {
    rtb_Gain_g = rtb_GainPhi;
  }

  rtb_Sum_l = rtb_Sum1_a / std::cos(FlyByWire_P.Gain1_Gain_b * rtb_Gain_g);
  FlyByWire_RateLimiter(FlyByWire_U.in.data.autopilot_custom_Theta_c_deg, FlyByWire_P.RateLimiterVariableTs1_up_k,
                        FlyByWire_P.RateLimiterVariableTs1_lo_h, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs1_InitialCondition_hb, &rtb_Y_h,
                        &FlyByWire_DWork.sf_RateLimiter_n);
  u1 = 5.0 - (FlyByWire_U.in.data.V_ias_kn - (FlyByWire_U.in.data.VLS_kn + 5.0)) * 0.25;
  if (0.0 > u1) {
    u1 = 0.0;
  }

  if (FlyByWire_U.in.data.flaps_handle_index == 5.0) {
    rtb_nz_limit_lo_g = 25;
  } else {
    rtb_nz_limit_lo_g = 30;
  }

  if (5.0 < u1) {
    u1 = 5.0;
  }

  FlyByWire_RateLimiter(static_cast<real_T>(rtb_nz_limit_lo_g) - u1, FlyByWire_P.RateLimiterVariableTs6_up,
                        FlyByWire_P.RateLimiterVariableTs6_lo, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs6_InitialCondition, &rtb_Y, &FlyByWire_DWork.sf_RateLimiter);
  FlyByWire_RateLimiter(rtb_BusAssignment_sim_input_delta_eta_pos, FlyByWire_P.RateLimiterVariableTs_up_f,
                        FlyByWire_P.RateLimiterVariableTs_lo_f, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs_InitialCondition_c, &rtb_Y_k,
                        &FlyByWire_DWork.sf_RateLimiter_k);
  u0_1 = rtb_BusAssignment_sim_input_delta_eta_pos - FlyByWire_DWork.Delay_DSTATE_i;
  u1 = FlyByWire_P.RateLimiterVariableTs3_up_m * FlyByWire_U.in.time.dt;
  if (u0_1 < u1) {
    u1 = u0_1;
  }

  rtb_Limiterxi1 = FlyByWire_U.in.time.dt * FlyByWire_P.RateLimiterVariableTs3_lo_e;
  if (u1 > rtb_Limiterxi1) {
    rtb_Limiterxi1 = u1;
  }

  FlyByWire_DWork.Delay_DSTATE_i += rtb_Limiterxi1;
  u0_1 = (rtb_LimiteriH - rtb_Min5) * FlyByWire_DWork.Delay_DSTATE_i;
  if (u0_1 <= 0.0) {
    u0_1 = 0.0;
  }

  rtb_v_target = rtb_LimiteriH + u0_1;
  rtb_Gain_g = FlyByWire_P.Subsystem2_Gain * rtb_v_target;
  rtb_Divide_m = (rtb_Gain_g - FlyByWire_DWork.Delay_DSTATE_j) / FlyByWire_U.in.time.dt;
  rtb_Sum1_a = FlyByWire_U.in.time.dt * FlyByWire_P.Subsystem2_C1;
  rtb_LimiteriH = rtb_Sum1_a + FlyByWire_P.Constant_Value_m3;
  FlyByWire_DWork.Delay1_DSTATE = 1.0 / rtb_LimiteriH * (FlyByWire_P.Constant_Value_m3 - rtb_Sum1_a) *
    FlyByWire_DWork.Delay1_DSTATE + (rtb_Divide_m + FlyByWire_DWork.Delay_DSTATE_c) * (rtb_Sum1_a / rtb_LimiteriH);
  rtb_Gain_c = FlyByWire_P.Subsystem_Gain * FlyByWire_U.in.data.V_ias_kn;
  rtb_Divide_e = (rtb_Gain_c - FlyByWire_DWork.Delay_DSTATE_p) / FlyByWire_U.in.time.dt;
  rtb_Sum1_a = FlyByWire_U.in.time.dt * FlyByWire_P.Subsystem_C1;
  rtb_LimiteriH = rtb_Sum1_a + FlyByWire_P.Constant_Value_h;
  FlyByWire_DWork.Delay1_DSTATE_i = 1.0 / rtb_LimiteriH * (FlyByWire_P.Constant_Value_h - rtb_Sum1_a) *
    FlyByWire_DWork.Delay1_DSTATE_i + (rtb_Divide_e + FlyByWire_DWork.Delay_DSTATE_m) * (rtb_Sum1_a / rtb_LimiteriH);
  u0_1 = rtb_Limiterxi - FlyByWire_DWork.Delay_DSTATE_g;
  u1 = FlyByWire_P.RateLimiterVariableTs4_up * FlyByWire_U.in.time.dt;
  if (u0_1 < u1) {
    u1 = u0_1;
  }

  rtb_Limiterxi1 = FlyByWire_U.in.time.dt * FlyByWire_P.RateLimiterVariableTs4_lo;
  if (u1 > rtb_Limiterxi1) {
    rtb_Limiterxi1 = u1;
  }

  FlyByWire_DWork.Delay_DSTATE_g += rtb_Limiterxi1;
  if (FlyByWire_U.in.data.autopilot_custom_on > FlyByWire_P.Switch1_Threshold_ke) {
    rtb_Sum1_a = (rtb_Y_h - rtb_GainTheta) * FlyByWire_P.Gain4_Gain;
  } else {
    rtb_LimiteriH = FlyByWire_P.Gain1_Gain * rtb_GainTheta;
    rtb_Sum1_a = rtb_Divide1 - rtb_Sum_l;
    rtb_Y_h = look1_binlxpw(rtb_Y_k, FlyByWire_P.Loaddemand_bp01Data, FlyByWire_P.Loaddemand_tableData, 2U);
    if (FlyByWire_DWork.Delay_DSTATE_g > FlyByWire_P.Saturation_UpperSat_o) {
      rtb_Y_k = FlyByWire_P.Saturation_UpperSat_o;
    } else if (FlyByWire_DWork.Delay_DSTATE_g < FlyByWire_P.Saturation_LowerSat_k) {
      rtb_Y_k = FlyByWire_P.Saturation_LowerSat_k;
    } else {
      rtb_Y_k = FlyByWire_DWork.Delay_DSTATE_g;
    }

    if (rtb_Limiterxi > FlyByWire_P.Switch2_Threshold) {
      u0_1 = (((((rtb_v_target - FlyByWire_U.in.data.V_ias_kn) * FlyByWire_P.Gain6_Gain +
                 FlyByWire_P.precontrol_gain_HSP_Gain * FlyByWire_DWork.Delay1_DSTATE) + FlyByWire_P.v_dot_gain_HSP_Gain
                * FlyByWire_DWork.Delay1_DSTATE_i) + FlyByWire_P.qk_gain_HSP_Gain *
               rtb_BusAssignment_cs_sim_data_qk_deg_s) + FlyByWire_P.qk_dot_gain1_Gain *
              rtb_BusAssignment_a_sim_data_qk_dot_deg_s2) * FlyByWire_P.HSP_gain_Gain;
      if (rtb_Y_h > FlyByWire_P.Saturation8_UpperSat) {
        rtb_Limiterxi = FlyByWire_P.Saturation8_UpperSat;
      } else if (rtb_Y_h < FlyByWire_P.Saturation8_LowerSat) {
        rtb_Limiterxi = FlyByWire_P.Saturation8_LowerSat;
      } else {
        rtb_Limiterxi = rtb_Y_h;
      }

      if (u0_1 > FlyByWire_P.Saturation4_UpperSat) {
        u0_1 = FlyByWire_P.Saturation4_UpperSat;
      } else {
        if (u0_1 < FlyByWire_P.Saturation4_LowerSat) {
          u0_1 = FlyByWire_P.Saturation4_LowerSat;
        }
      }

      rtb_Limiterxi += u0_1;
    } else {
      rtb_Limiterxi = FlyByWire_P.Constant1_Value;
    }

    rtb_Y_h = (FlyByWire_P.Constant_Value_k - rtb_Y_k) * rtb_Y_h + rtb_Limiterxi * rtb_Y_k;
    if (rtb_in_flare > FlyByWire_P.Switch_Threshold) {
      rtb_Limiterxi = (FlyByWire_DWork.Delay_DSTATE_dq - rtb_GainTheta) * FlyByWire_P.Gain_Gain;
      if (rtb_Limiterxi > FlyByWire_P.Saturation_UpperSat) {
        rtb_Limiterxi = FlyByWire_P.Saturation_UpperSat;
      } else {
        if (rtb_Limiterxi < FlyByWire_P.Saturation_LowerSat) {
          rtb_Limiterxi = FlyByWire_P.Saturation_LowerSat;
        }
      }
    } else {
      rtb_Limiterxi = FlyByWire_P.Constant_Value_m;
    }

    u0_1 = FlyByWire_P.Gain2_Gain * rtb_Y - rtb_LimiteriH;
    if (u0_1 > FlyByWire_P.Saturation1_UpperSat) {
      u0_1 = FlyByWire_P.Saturation1_UpperSat;
    } else {
      if (u0_1 < FlyByWire_P.Saturation1_LowerSat) {
        u0_1 = FlyByWire_P.Saturation1_LowerSat;
      }
    }

    rtb_Y_k = look1_binlxpw(u0_1, FlyByWire_P.Loaddemand1_bp01Data, FlyByWire_P.Loaddemand1_tableData, 2U) + rtb_Sum1_a;
    if (rtb_Y_h <= rtb_Y_k) {
      u0_1 = FlyByWire_P.Gain3_Gain * FlyByWire_P.Theta_max3_Value - rtb_LimiteriH;
      if (u0_1 > FlyByWire_P.Saturation2_UpperSat) {
        u0_1 = FlyByWire_P.Saturation2_UpperSat;
      } else {
        if (u0_1 < FlyByWire_P.Saturation2_LowerSat) {
          u0_1 = FlyByWire_P.Saturation2_LowerSat;
        }
      }

      rtb_Y_k = look1_binlxpw(u0_1, FlyByWire_P.Loaddemand2_bp01Data, FlyByWire_P.Loaddemand2_tableData, 2U) +
        rtb_Sum1_a;
      if (rtb_Y_h >= rtb_Y_k) {
        rtb_Y_k = rtb_Y_h;
      }
    }

    rtb_Sum1_a = rtb_Y_k + rtb_Limiterxi;
  }

  rtb_Sum1_a += rtb_Sum_l;
  if (rtb_Sum1_a > rtb_Y_o) {
    rtb_Sum1_a = rtb_Y_o;
  } else {
    if (rtb_Sum1_a < rtb_Y_dd) {
      rtb_Sum1_a = rtb_Y_dd;
    }
  }

  rtb_LimiteriH = FlyByWire_P.Gain1_Gain_j * rtb_BusAssignment_cs_sim_data_qk_deg_s * (FlyByWire_P.Gain_Gain_dc *
    FlyByWire_P.Vm_currentms_Value) + (FlyByWire_U.in.data.nz_g - rtb_Divide1);
  if (FlyByWire_U.in.data.V_tas_kn > FlyByWire_P.Saturation3_UpperSat) {
    u0_1 = FlyByWire_P.Saturation3_UpperSat;
  } else if (FlyByWire_U.in.data.V_tas_kn < FlyByWire_P.Saturation3_LowerSat) {
    u0_1 = FlyByWire_P.Saturation3_LowerSat;
  } else {
    u0_1 = FlyByWire_U.in.data.V_tas_kn;
  }

  rtb_Limiterxi = rtb_LimiteriH - (look1_binlxpw(FlyByWire_U.in.data.V_tas_kn, FlyByWire_P.uDLookupTable_bp01Data_j,
    FlyByWire_P.uDLookupTable_tableData_l, 4U) / (FlyByWire_P.Gain5_Gain * u0_1) + FlyByWire_P.Bias_Bias_d) *
    (rtb_Sum1_a - rtb_Divide1);
  rtb_Product1_n5 = rtb_Limiterxi * look1_binlxpw(FlyByWire_U.in.data.V_tas_kn, FlyByWire_P.PLUT_bp01Data,
    FlyByWire_P.PLUT_tableData, 1U);
  rtb_Y = FlyByWire_P.DiscreteDerivativeVariableTs1_Gain * rtb_BusAssignment_cs_sim_data_qk_deg_s;
  rtb_Y_k = rtb_Limiterxi * look1_binlxpw(FlyByWire_U.in.data.V_tas_kn, FlyByWire_P.DLUT_bp01Data,
    FlyByWire_P.DLUT_tableData, 1U) * FlyByWire_P.DiscreteDerivativeVariableTs_Gain_e;
  u0_1 = static_cast<real_T>(rtb_AND) - FlyByWire_DWork.Delay_DSTATE_k;
  u1 = FlyByWire_P.RateLimiterVariableTs5_up * FlyByWire_U.in.time.dt;
  if (u0_1 < u1) {
    u1 = u0_1;
  }

  rtb_Limiterxi1 = FlyByWire_U.in.time.dt * FlyByWire_P.RateLimiterVariableTs5_lo;
  if (u1 > rtb_Limiterxi1) {
    rtb_Limiterxi1 = u1;
  }

  FlyByWire_DWork.Delay_DSTATE_k += rtb_Limiterxi1;
  if (FlyByWire_DWork.Delay_DSTATE_k > FlyByWire_P.Saturation_UpperSat_a) {
    rtb_Limiterxi1 = FlyByWire_P.Saturation_UpperSat_a;
  } else if (FlyByWire_DWork.Delay_DSTATE_k < FlyByWire_P.Saturation_LowerSat_ps) {
    rtb_Limiterxi1 = FlyByWire_P.Saturation_LowerSat_ps;
  } else {
    rtb_Limiterxi1 = FlyByWire_DWork.Delay_DSTATE_k;
  }

  FlyByWire_RateLimiter(rtb_BusAssignment_sim_input_delta_eta_pos, FlyByWire_P.RateLimiterVariableTs2_up_b,
                        FlyByWire_P.RateLimiterVariableTs2_lo_n, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs2_InitialCondition_j, &rtb_Y_dd,
                        &FlyByWire_DWork.sf_RateLimiter_p);
  rtb_y_g = (rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_max_deg - rtb_Switch) * rtb_Y_dd;
  FlyByWire_LagFilter(FlyByWire_U.in.data.alpha_deg, FlyByWire_P.LagFilter1_C1, FlyByWire_U.in.time.dt, &rtb_Y_dd,
                      &FlyByWire_DWork.sf_LagFilter_h);
  rtb_Limiterxi = rtb_Y_dd - rtb_Switch;
  if (0.0 > rtb_GainTheta - 22.5) {
    rtb_Y_dd = 0.0;
  } else {
    rtb_Y_dd = rtb_GainTheta - 22.5;
  }

  u1 = (std::abs(rtb_GainPhi) - 3.0) / 6.0;
  if (0.0 > u1) {
    u1 = 0.0;
  }

  if (rtb_Y_dd > u1) {
    u1 = rtb_Y_dd;
  }

  FlyByWire_WashoutFilter(u1, FlyByWire_P.WashoutFilter_C1, FlyByWire_U.in.time.dt, &rtb_Y_dd,
    &FlyByWire_DWork.sf_WashoutFilter);
  rtb_Limiterxi = (rtb_y_g - rtb_Limiterxi) - rtb_Y_dd;
  rtb_Y_h = FlyByWire_P.Subsystem1_Gain * rtb_Limiterxi;
  rtb_Divide1 = (rtb_Y_h - FlyByWire_DWork.Delay_DSTATE_ps) / FlyByWire_U.in.time.dt;
  rtb_Limiterxi2 = FlyByWire_U.in.time.dt * FlyByWire_P.Subsystem1_C1;
  rtb_Sum_l = rtb_Limiterxi2 + FlyByWire_P.Constant_Value_kr;
  FlyByWire_DWork.Delay1_DSTATE_o = 1.0 / rtb_Sum_l * (FlyByWire_P.Constant_Value_kr - rtb_Limiterxi2) *
    FlyByWire_DWork.Delay1_DSTATE_o + (rtb_Divide1 + FlyByWire_DWork.Delay_DSTATE_c1) * (rtb_Limiterxi2 / rtb_Sum_l);
  rtb_Y_dd = FlyByWire_P.alpha_err_gain_Gain * rtb_Limiterxi;
  rtb_Sum_l = FlyByWire_P.Subsystem3_Gain * FlyByWire_U.in.data.V_ias_kn;
  rtb_Divide_b = (rtb_Sum_l - FlyByWire_DWork.Delay_DSTATE_l) / FlyByWire_U.in.time.dt;
  rtb_Limiterxi = FlyByWire_U.in.time.dt * FlyByWire_P.Subsystem3_C1;
  rtb_Limiterxi2 = rtb_Limiterxi + FlyByWire_P.Constant_Value_c;
  FlyByWire_DWork.Delay1_DSTATE_n = 1.0 / rtb_Limiterxi2 * (FlyByWire_P.Constant_Value_c - rtb_Limiterxi) *
    FlyByWire_DWork.Delay1_DSTATE_n + (rtb_Divide_b + FlyByWire_DWork.Delay_DSTATE_n) * (rtb_Limiterxi / rtb_Limiterxi2);
  FlyByWire_Y.out.pitch.law_normal.Cstar_g = rtb_LimiteriH;
  u0_1 = (((FlyByWire_P.precontrol_gain_Gain * FlyByWire_DWork.Delay1_DSTATE_o + rtb_Y_dd) + FlyByWire_P.v_dot_gain_Gain
           * FlyByWire_DWork.Delay1_DSTATE_n) + FlyByWire_P.qk_gain_Gain * rtb_BusAssignment_cs_sim_data_qk_deg_s) +
    FlyByWire_P.qk_dot_gain_Gain * rtb_BusAssignment_a_sim_data_qk_dot_deg_s2;
  rtb_Limiterxi = ((rtb_Y - FlyByWire_DWork.Delay_DSTATE_ca) / FlyByWire_U.in.time.dt * FlyByWire_P.Gain3_Gain_l +
                   rtb_Product1_n5) + (rtb_Y_k - FlyByWire_DWork.Delay_DSTATE_jv) / FlyByWire_U.in.time.dt;
  if (u0_1 > FlyByWire_P.Saturation3_UpperSat_c) {
    u0_1 = FlyByWire_P.Saturation3_UpperSat_c;
  } else {
    if (u0_1 < FlyByWire_P.Saturation3_LowerSat_h) {
      u0_1 = FlyByWire_P.Saturation3_LowerSat_h;
    }
  }

  if (rtb_Limiterxi > FlyByWire_P.Saturation_UpperSat_j) {
    rtb_Limiterxi = FlyByWire_P.Saturation_UpperSat_j;
  } else {
    if (rtb_Limiterxi < FlyByWire_P.Saturation_LowerSat_c) {
      rtb_Limiterxi = FlyByWire_P.Saturation_LowerSat_c;
    }
  }

  rtb_BusAssignment_f_pitch_law_normal_eta_dot_deg_s = (FlyByWire_P.Constant_Value_p - rtb_Limiterxi1) * rtb_Limiterxi +
    u0_1 * rtb_Limiterxi1;
  FlyByWire_Y.out.pitch.law_normal.nz_c_g = rtb_Sum1_a;
  FlyByWire_RateLimiter(rtb_BusAssignment_sim_input_delta_eta_pos, FlyByWire_P.RateLimiterVariableTs_up_dl,
                        FlyByWire_P.RateLimiterVariableTs_lo_d, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs_InitialCondition_n, &rtb_Y_dd,
                        &FlyByWire_DWork.sf_RateLimiter_l);
  if (rtb_Y_dd > FlyByWire_P.Saturation3_UpperSat_e) {
    rtb_Sum1_a = FlyByWire_P.Saturation3_UpperSat_e;
  } else if (rtb_Y_dd < FlyByWire_P.Saturation3_LowerSat_c) {
    rtb_Sum1_a = FlyByWire_P.Saturation3_LowerSat_c;
  } else {
    rtb_Sum1_a = rtb_Y_dd;
  }

  rtb_Sum1_a = look1_binlxpw(static_cast<real_T>(FlyByWire_U.in.data.tailstrike_protection_on) * look2_binlxpw
    (rtb_GainTheta, FlyByWire_U.in.data.H_radio_ft, FlyByWire_P.uDLookupTable_bp01Data_l,
     FlyByWire_P.uDLookupTable_bp02Data, FlyByWire_P.uDLookupTable_tableData_d, FlyByWire_P.uDLookupTable_maxIndex, 5U) *
    rtb_Sum1_a + rtb_Y_dd, FlyByWire_P.PitchRateDemand_bp01Data, FlyByWire_P.PitchRateDemand_tableData, 2U);
  FlyByWire_Y.out.pitch.law_rotation.qk_c_deg_s = rtb_Sum1_a;
  rtb_Limiterxi2 = FlyByWire_P.DiscreteDerivativeVariableTs_Gain_c * rtb_Sum1_a;
  rtb_LimiteriH = rtb_BusAssignment_cs_sim_data_qk_deg_s - rtb_Sum1_a;
  rtb_Product1_n5 = FlyByWire_P.Gain1_Gain_i * rtb_LimiteriH * FlyByWire_P.DiscreteDerivativeVariableTs_Gain_b;
  FlyByWire_LagFilter(rtb_BusAssignment_cs_sim_data_qk_deg_s + FlyByWire_P.Gain5_Gain_m *
                      rtb_BusAssignment_a_sim_data_qk_dot_deg_s2, FlyByWire_P.LagFilter_C1_i, FlyByWire_U.in.time.dt,
                      &rtb_Y_dd, &FlyByWire_DWork.sf_LagFilter_p);
  rtb_Sum1_a = (((((rtb_Product1_n5 - FlyByWire_DWork.Delay_DSTATE_dd) / FlyByWire_U.in.time.dt +
                   FlyByWire_P.Gain_Gain_h * rtb_LimiteriH) * FlyByWire_P.Gain1_Gain_a + (rtb_Limiterxi2 -
    FlyByWire_DWork.Delay_DSTATE_f) / FlyByWire_U.in.time.dt * FlyByWire_P.Gain3_Gain_p) + (rtb_Y_dd - rtb_Sum1_a) *
                 FlyByWire_P.Gain4_Gain_g) + FlyByWire_P.Gain6_Gain_f * rtb_BusAssignment_a_sim_data_qk_dot_deg_s2) *
    FlyByWire_P.DiscreteTimeIntegratorVariableTs_Gain * FlyByWire_U.in.time.dt;
  if (((rtb_BusAssignment_sim_input_delta_eta_pos <= FlyByWire_P.Constant_Value_j) && (rtb_on_ground != 0)) || (rtb_Y_e ==
       0.0) || (rtb_alpha_floor_inhib != 0)) {
    FlyByWire_DWork.icLoad = 1U;
  }

  if (FlyByWire_DWork.icLoad != 0) {
    FlyByWire_DWork.Delay_DSTATE_e = FlyByWire_P.Constant_Value_hn - rtb_Sum1_a;
  }

  rtb_Sum1_a += FlyByWire_DWork.Delay_DSTATE_e;
  if (rtb_Sum1_a > FlyByWire_P.DiscreteTimeIntegratorVariableTs_UpperLimit) {
    FlyByWire_DWork.Delay_DSTATE_e = FlyByWire_P.DiscreteTimeIntegratorVariableTs_UpperLimit;
  } else if (rtb_Sum1_a < FlyByWire_P.DiscreteTimeIntegratorVariableTs_LowerLimit) {
    FlyByWire_DWork.Delay_DSTATE_e = FlyByWire_P.DiscreteTimeIntegratorVariableTs_LowerLimit;
  } else {
    FlyByWire_DWork.Delay_DSTATE_e = rtb_Sum1_a;
  }

  if (rtb_on_ground > FlyByWire_P.Switch_Threshold_he) {
    if (rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg > FlyByWire_P.Saturation_UpperSat_g) {
      rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_g;
    } else if (rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg < FlyByWire_P.Saturation_LowerSat_p) {
      rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_p;
    } else {
      rtb_Sum1_a = rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg;
    }
  } else {
    rtb_Sum1_a = FlyByWire_P.Constant1_Value_i;
  }

  rtb_BusAssignment_cs_sim_data_qk_deg_s = FlyByWire_DWork.Delay_DSTATE_e + rtb_Sum1_a;
  rtb_Sum1_a = FlyByWire_P.DiscreteTimeIntegratorVariableTs_Gain_k * rtb_BusAssignment_f_pitch_law_normal_eta_dot_deg_s *
    FlyByWire_U.in.time.dt;
  if ((rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain == 0.0) || (rtb_alpha_floor_inhib != 0)) {
    FlyByWire_DWork.icLoad_e = 1U;
  }

  if (FlyByWire_DWork.icLoad_e != 0) {
    if (FlyByWire_B.in_flight > FlyByWire_P.Switch_Threshold_d) {
      u0_1 = rtb_Gainpk4;
    } else {
      u0_1 = rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg;
    }

    FlyByWire_DWork.Delay_DSTATE_f1 = u0_1 - rtb_Sum1_a;
  }

  rtb_Sum1_a += FlyByWire_DWork.Delay_DSTATE_f1;
  if (rtb_Sum1_a > FlyByWire_P.DiscreteTimeIntegratorVariableTs_UpperLimit_c) {
    FlyByWire_DWork.Delay_DSTATE_f1 = FlyByWire_P.DiscreteTimeIntegratorVariableTs_UpperLimit_c;
  } else if (rtb_Sum1_a < FlyByWire_P.DiscreteTimeIntegratorVariableTs_LowerLimit_b) {
    FlyByWire_DWork.Delay_DSTATE_f1 = FlyByWire_P.DiscreteTimeIntegratorVariableTs_LowerLimit_b;
  } else {
    FlyByWire_DWork.Delay_DSTATE_f1 = rtb_Sum1_a;
  }

  if (rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain > FlyByWire_P.Saturation_UpperSat_g4) {
    rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_g4;
  } else if (rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain < FlyByWire_P.Saturation_LowerSat_l) {
    rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_l;
  } else {
    rtb_Sum1_a = rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain;
  }

  rtb_LimiteriH = FlyByWire_DWork.Delay_DSTATE_f1 * rtb_Sum1_a;
  rtb_Y_dd = FlyByWire_P.Constant_Value_o - rtb_Sum1_a;
  if (rtb_Y_e > FlyByWire_P.Saturation_UpperSat_c) {
    rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_c;
  } else if (rtb_Y_e < FlyByWire_P.Saturation_LowerSat_m) {
    rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_m;
  } else {
    rtb_Sum1_a = rtb_Y_e;
  }

  rtb_Limiterxi = rtb_BusAssignment_cs_sim_data_qk_deg_s * rtb_Sum1_a;
  rtb_Sum1_a = FlyByWire_P.Constant_Value_ju - rtb_Sum1_a;
  rtb_Sum1_a *= rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg;
  rtb_BusAssignment_m_pitch_output_eta_deg = (rtb_Limiterxi + rtb_Sum1_a) * rtb_Y_dd + rtb_LimiteriH;
  if (rtb_eta_trim_deg_should_freeze == FlyByWire_P.CompareToConstant_const_hh) {
    rtb_Limiterxi = FlyByWire_P.Constant_Value;
  } else {
    rtb_Limiterxi = FlyByWire_DWork.Delay_DSTATE_f1;
  }

  rtb_Sum1_a = FlyByWire_P.Gain_Gain_ip * rtb_Limiterxi * FlyByWire_P.DiscreteTimeIntegratorVariableTsLimit_Gain *
    FlyByWire_U.in.time.dt;
  if (rtb_eta_trim_deg_reset) {
    FlyByWire_DWork.icLoad_i = 1U;
  }

  if (FlyByWire_DWork.icLoad_i != 0) {
    FlyByWire_DWork.Delay_DSTATE_h = rtb_nz_limit_up_g - rtb_Sum1_a;
  }

  FlyByWire_DWork.Delay_DSTATE_h += rtb_Sum1_a;
  if (FlyByWire_DWork.Delay_DSTATE_h > rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_up) {
    FlyByWire_DWork.Delay_DSTATE_h = rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_up;
  } else {
    if (FlyByWire_DWork.Delay_DSTATE_h < rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_lo) {
      FlyByWire_DWork.Delay_DSTATE_h = rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_lo;
    }
  }

  u0_1 = FlyByWire_DWork.Delay_DSTATE_h - FlyByWire_DWork.Delay_DSTATE_ea;
  u1 = rtb_ManualSwitch * FlyByWire_U.in.time.dt;
  if (u0_1 < u1) {
    u1 = u0_1;
  }

  rtb_Limiterxi1 = FlyByWire_U.in.time.dt * rtb_eta_trim_deg_rate_limit_lo_deg_s;
  if (u1 > rtb_Limiterxi1) {
    rtb_Limiterxi1 = u1;
  }

  FlyByWire_DWork.Delay_DSTATE_ea += rtb_Limiterxi1;
  FlyByWire_Y.out.pitch.data_computed.in_rotation_gain = rtb_Y_e;
  FlyByWire_Y.out.pitch.data_computed.nz_limit_up_g = rtb_Y_o;
  rtb_Y_e = look1_binlxpw(FlyByWire_U.in.data.V_tas_kn, FlyByWire_P.uDLookupTable_bp01Data_f,
    FlyByWire_P.uDLookupTable_tableData_f, 3U);
  rtb_Gain1_mp = FlyByWire_P.Gain1_Gain_jh * rtb_BusAssignment_sim_input_delta_zeta_pos;
  if (rtb_Gain1_mp > rtb_Y_e) {
    rtb_Gain1_mp = rtb_Y_e;
  } else {
    rtb_Y_e *= FlyByWire_P.Gain2_Gain_n;
    if (rtb_Gain1_mp < rtb_Y_e) {
      rtb_Gain1_mp = rtb_Y_e;
    }
  }

  if (FlyByWire_DWork.is_active_c5_FlyByWire == 0U) {
    FlyByWire_DWork.is_active_c5_FlyByWire = 1U;
    FlyByWire_DWork.is_c5_FlyByWire = FlyByWire_IN_GroundMode;
    rtb_nz_limit_lo_g = 0;
  } else if (FlyByWire_DWork.is_c5_FlyByWire == 1) {
    if (rtb_on_ground == 1) {
      FlyByWire_DWork.is_c5_FlyByWire = FlyByWire_IN_GroundMode;
      rtb_nz_limit_lo_g = 0;
    } else {
      rtb_nz_limit_lo_g = 1;
    }
  } else {
    if (((rtb_on_ground == 0) && (rtb_GainTheta > 8.0)) || (FlyByWire_U.in.data.H_radio_ft > 400.0)) {
      FlyByWire_DWork.is_c5_FlyByWire = FlyByWire_IN_FlightMode;
      rtb_nz_limit_lo_g = 1;
    } else {
      rtb_nz_limit_lo_g = 0;
    }
  }

  if (rtb_nz_limit_lo_g > FlyByWire_P.Saturation_UpperSat_p) {
    rtb_Y_e = FlyByWire_P.Saturation_UpperSat_p;
  } else if (rtb_nz_limit_lo_g < FlyByWire_P.Saturation_LowerSat_h) {
    rtb_Y_e = FlyByWire_P.Saturation_LowerSat_h;
  } else {
    rtb_Y_e = rtb_nz_limit_lo_g;
  }

  FlyByWire_RateLimiter(rtb_Y_e, FlyByWire_P.RateLimiterVariableTs_up_k, FlyByWire_P.RateLimiterVariableTs_lo_fs,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs_InitialCondition_f, &rtb_Y_dd,
                        &FlyByWire_DWork.sf_RateLimiter_gp);
  FlyByWire_LagFilter(FlyByWire_U.in.data.engine_1_thrust_lbf - FlyByWire_U.in.data.engine_2_thrust_lbf,
                      FlyByWire_P.LagFilter_C1_l, FlyByWire_U.in.time.dt, &rtb_Y_o, &FlyByWire_DWork.sf_LagFilter_pl);
  rtb_Product_e = rtb_Y_o * look1_binlxpw(FlyByWire_U.in.data.V_tas_kn,
    FlyByWire_P.ScheduledGain_BreakpointsForDimension1, FlyByWire_P.ScheduledGain_Table, 3U);
  rtb_BusAssignment_p_roll_data_computed_delta_xi_deg = FlyByWire_P.Gain_Gain_c *
    rtb_BusAssignment_sim_input_delta_xi_pos;
  rtb_BusAssignment_p_roll_data_computed_in_flight_gain = rtb_Y_dd;
  if (rtb_ap_special_disc > FlyByWire_P.Switch3_Threshold) {
    rtb_Y_e = look1_binlxpw(rtb_GainPhi, FlyByWire_P.BankAngleProtection2_bp01Data,
      FlyByWire_P.BankAngleProtection2_tableData, 4U);
  } else if (static_cast<real_T>(rtb_AND) > FlyByWire_P.Switch2_Threshold_f) {
    rtb_Y_e = look1_binlxpw(rtb_GainPhi, FlyByWire_P.BankAngleProtection_bp01Data,
      FlyByWire_P.BankAngleProtection_tableData, 8U);
  } else {
    rtb_Y_e = look1_binlxpw(rtb_GainPhi, FlyByWire_P.BankAngleProtection1_bp01Data,
      FlyByWire_P.BankAngleProtection1_tableData, 8U);
  }

  u0_1 = FlyByWire_P.Gain1_Gain_m * rtb_BusAssignment_sim_input_delta_xi_pos + rtb_Y_e;
  if (u0_1 > FlyByWire_P.Saturation_UpperSat_n) {
    u0_1 = FlyByWire_P.Saturation_UpperSat_n;
  } else {
    if (u0_1 < FlyByWire_P.Saturation_LowerSat_o) {
      u0_1 = FlyByWire_P.Saturation_LowerSat_o;
    }
  }

  rtb_Sum1_a = u0_1 * rtb_Y_dd;
  rtb_LimiteriH = FlyByWire_P.DiscreteTimeIntegratorVariableTs_Gain_d * rtb_Sum1_a * FlyByWire_U.in.time.dt;
  if ((rtb_Y_dd == 0.0) || (rtb_alpha_floor_inhib != 0) || (FlyByWire_U.in.data.autopilot_custom_on != 0.0)) {
    FlyByWire_DWork.icLoad_m = 1U;
  }

  if (FlyByWire_DWork.icLoad_m != 0) {
    FlyByWire_DWork.Delay_DSTATE_ho = rtb_GainPhi - rtb_LimiteriH;
  }

  rtb_LimiteriH += FlyByWire_DWork.Delay_DSTATE_ho;
  if (rtb_LimiteriH > FlyByWire_P.DiscreteTimeIntegratorVariableTs_UpperLimit_d) {
    FlyByWire_DWork.Delay_DSTATE_ho = FlyByWire_P.DiscreteTimeIntegratorVariableTs_UpperLimit_d;
  } else if (rtb_LimiteriH < FlyByWire_P.DiscreteTimeIntegratorVariableTs_LowerLimit_k) {
    FlyByWire_DWork.Delay_DSTATE_ho = FlyByWire_P.DiscreteTimeIntegratorVariableTs_LowerLimit_k;
  } else {
    FlyByWire_DWork.Delay_DSTATE_ho = rtb_LimiteriH;
  }

  if (FlyByWire_U.in.data.autopilot_custom_on > FlyByWire_P.Switch_Threshold_p) {
    if (rtb_alpha_floor_inhib > FlyByWire_P.Switch1_Threshold) {
      u0_1 = rtb_GainPhi;
    } else {
      u0_1 = FlyByWire_U.in.data.autopilot_custom_Phi_c_deg;
    }
  } else {
    u0_1 = FlyByWire_DWork.Delay_DSTATE_ho;
  }

  FlyByWire_RateLimiter(u0_1, FlyByWire_P.RateLimiterVariableTs2_up_bi, FlyByWire_P.RateLimiterVariableTs2_lo_a,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs2_InitialCondition_l, &rtb_Y_o,
                        &FlyByWire_DWork.sf_RateLimiter_j);
  FlyByWire_RateLimiter(rtb_Gain1_mp, FlyByWire_P.RateLimiterVariableTs_up_m, FlyByWire_P.RateLimiterVariableTs_lo_p,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterVariableTs_InitialCondition_fc, &rtb_Y_e,
                        &FlyByWire_DWork.sf_RateLimiter_lp);
  FlyByWire_LagFilter(rtb_Gain, FlyByWire_P.LagFilter_C1_d, FlyByWire_U.in.time.dt, &rtb_Y_dd,
                      &FlyByWire_DWork.sf_LagFilter_c);
  if (FlyByWire_U.in.data.V_tas_kn > FlyByWire_P.Saturation_UpperSat_l) {
    u0_1 = FlyByWire_P.Saturation_UpperSat_l;
  } else if (FlyByWire_U.in.data.V_tas_kn < FlyByWire_P.Saturation_LowerSat_lu) {
    u0_1 = FlyByWire_P.Saturation_LowerSat_lu;
  } else {
    u0_1 = FlyByWire_U.in.data.V_tas_kn;
  }

  rtb_Limiterxi = rtb_Y_dd - std::sin(FlyByWire_P.Gain1_Gain_br * rtb_Y_o) * FlyByWire_P.Constant2_Value_l * std::cos
    (FlyByWire_P.Gain1_Gain_cq * rtb_GainTheta) / (FlyByWire_P.Gain6_Gain_j * u0_1) * FlyByWire_P.Gain_Gain_cd;
  FlyByWire_WashoutFilter(rtb_Gain, FlyByWire_P.WashoutFilter_C1_i, FlyByWire_U.in.time.dt, &rtb_Y_dd,
    &FlyByWire_DWork.sf_WashoutFilter_i);
  rtb_Limiterxi1 = FlyByWire_P.Gain6_Gain_k * rtb_Y_dd;
  if (rtb_Limiterxi1 > FlyByWire_P.Saturation2_UpperSat_e) {
    rtb_Limiterxi1 = FlyByWire_P.Saturation2_UpperSat_e;
  } else {
    if (rtb_Limiterxi1 < FlyByWire_P.Saturation2_LowerSat_gp) {
      rtb_Limiterxi1 = FlyByWire_P.Saturation2_LowerSat_gp;
    }
  }

  u0_1 = FlyByWire_P.Gain_Gain_hk * rtb_Limiterxi;
  if (u0_1 > FlyByWire_P.Saturation1_UpperSat_h) {
    u0_1 = FlyByWire_P.Saturation1_UpperSat_h;
  } else {
    if (u0_1 < FlyByWire_P.Saturation1_LowerSat_g) {
      u0_1 = FlyByWire_P.Saturation1_LowerSat_g;
    }
  }

  rtb_Limiterxi = rtb_BusAssignment_p_roll_data_computed_in_flight_gain * u0_1 + rtb_Limiterxi1;
  FlyByWire_Y.out.roll.law_normal.pk_c_deg_s = rtb_Sum1_a;
  FlyByWire_Y.out.roll.law_normal.Phi_c_deg = rtb_Y_o;
  rtb_BusAssignment_e_roll_law_normal_xi_deg = ((FlyByWire_P.Gain3_Gain_k * rtb_Y_e + rtb_Y_o) - rtb_GainPhi) *
    FlyByWire_P.Gain2_Gain_i + FlyByWire_P.Gain1_Gain_mg * rtb_BusAssignment_a_sim_data_pk_deg_s * FlyByWire_P.pKp_Gain;
  FlyByWire_Y.out.roll.law_normal.zeta_tc_yd_deg = rtb_Limiterxi;
  FlyByWire_RateLimiter(static_cast<real_T>(rtb_on_ground), FlyByWire_P.RateLimiterVariableTs_up_dlj,
                        FlyByWire_P.RateLimiterVariableTs_lo_fw, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs_InitialCondition_p, &rtb_Y_dd,
                        &FlyByWire_DWork.sf_RateLimiter_d);
  if (rtb_Y_dd > FlyByWire_P.Saturation_UpperSat_d1) {
    rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_d1;
  } else if (rtb_Y_dd < FlyByWire_P.Saturation_LowerSat_j) {
    rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_j;
  } else {
    rtb_Sum1_a = rtb_Y_dd;
  }

  rtb_LimiteriH = FlyByWire_U.in.data.autopilot_custom_Beta_c_deg * rtb_Sum1_a;
  rtb_Limiterxi1 = FlyByWire_P.Constant_Value_ie - rtb_Sum1_a;
  FlyByWire_LagFilter(FlyByWire_U.in.data.beta_deg, FlyByWire_P.LagFilter_C1_c, FlyByWire_U.in.time.dt, &rtb_Y_o,
                      &FlyByWire_DWork.sf_LagFilter_d);
  FlyByWire_LagFilter(FlyByWire_U.in.data.autopilot_custom_Beta_c_deg + rtb_Product_e, FlyByWire_P.LagFilter1_C1_f,
                      FlyByWire_U.in.time.dt, &rtb_Y_dd, &FlyByWire_DWork.sf_LagFilter_k);
  rtb_AND = ((rtb_alpha_floor_inhib == 0) && (FlyByWire_U.in.data.autopilot_custom_on != 0.0));
  if (!rtb_AND) {
    rtb_Y_dd = rtb_Y_o;
  }

  rtb_Sum1_a = rtb_Y_dd - rtb_Y_o;
  rtb_Y_o = FlyByWire_P.Gain4_Gain_o * rtb_Sum1_a;
  rtb_Sum1_a = FlyByWire_P.Gain7_Gain * rtb_Sum1_a * FlyByWire_P.DiscreteTimeIntegratorVariableTs1_Gain *
    FlyByWire_U.in.time.dt;
  if (!rtb_AND) {
    FlyByWire_DWork.icLoad_id = 1U;
  }

  if (FlyByWire_DWork.icLoad_id != 0) {
    FlyByWire_DWork.Delay_DSTATE_gt = FlyByWire_P.fbw_output_MATLABStruct.roll.output.zeta_deg - rtb_Sum1_a;
  }

  rtb_Sum1_a += FlyByWire_DWork.Delay_DSTATE_gt;
  if (rtb_Sum1_a > FlyByWire_P.DiscreteTimeIntegratorVariableTs1_UpperLimit) {
    FlyByWire_DWork.Delay_DSTATE_gt = FlyByWire_P.DiscreteTimeIntegratorVariableTs1_UpperLimit;
  } else if (rtb_Sum1_a < FlyByWire_P.DiscreteTimeIntegratorVariableTs1_LowerLimit) {
    FlyByWire_DWork.Delay_DSTATE_gt = FlyByWire_P.DiscreteTimeIntegratorVariableTs1_LowerLimit;
  } else {
    FlyByWire_DWork.Delay_DSTATE_gt = rtb_Sum1_a;
  }

  FlyByWire_RateLimiter(rtb_Y_o + FlyByWire_DWork.Delay_DSTATE_gt, FlyByWire_P.RateLimiterVariableTs1_up_p,
                        FlyByWire_P.RateLimiterVariableTs1_lo_hd, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterVariableTs1_InitialCondition_f, &rtb_Y_dd,
                        &FlyByWire_DWork.sf_RateLimiter_bo);
  if (FlyByWire_U.in.data.autopilot_custom_on > FlyByWire_P.Saturation_UpperSat_k) {
    rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_k;
  } else if (FlyByWire_U.in.data.autopilot_custom_on < FlyByWire_P.Saturation_LowerSat_ae) {
    rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_ae;
  } else {
    rtb_Sum1_a = FlyByWire_U.in.data.autopilot_custom_on;
  }

  rtb_Sum1_a = ((rtb_Limiterxi1 * rtb_Y_dd + rtb_LimiteriH) * rtb_Sum1_a + (FlyByWire_P.Constant_Value_l - rtb_Sum1_a) *
                FlyByWire_P.Constant3_Value_l) + rtb_Limiterxi;
  rtb_Y_e += rtb_Sum1_a;
  if (FlyByWire_U.in.data.H_radio_ft <= FlyByWire_P.CompareToConstant_const_g) {
    rtb_Sum1_a = FlyByWire_P.Constant2_Value_c;
  }

  rtb_Sum1_a = FlyByWire_P.Gain4_Gain_h * rtb_Sum1_a * FlyByWire_P.DiscreteTimeIntegratorVariableTs1_Gain_e *
    FlyByWire_U.in.time.dt;
  if ((FlyByWire_U.in.data.autopilot_custom_on == 0.0) || (rtb_alpha_floor_inhib != 0)) {
    FlyByWire_DWork.icLoad_c = 1U;
  }

  if (FlyByWire_DWork.icLoad_c != 0) {
    FlyByWire_DWork.Delay_DSTATE_fz = rtb_BusAssignment_sim_data_zeta_trim_deg - rtb_Sum1_a;
  }

  rtb_Sum1_a += FlyByWire_DWork.Delay_DSTATE_fz;
  if (rtb_Sum1_a > FlyByWire_P.DiscreteTimeIntegratorVariableTs1_UpperLimit_e) {
    FlyByWire_DWork.Delay_DSTATE_fz = FlyByWire_P.DiscreteTimeIntegratorVariableTs1_UpperLimit_e;
  } else if (rtb_Sum1_a < FlyByWire_P.DiscreteTimeIntegratorVariableTs1_LowerLimit_o) {
    FlyByWire_DWork.Delay_DSTATE_fz = FlyByWire_P.DiscreteTimeIntegratorVariableTs1_LowerLimit_o;
  } else {
    FlyByWire_DWork.Delay_DSTATE_fz = rtb_Sum1_a;
  }

  u0_1 = FlyByWire_DWork.Delay_DSTATE_fz - FlyByWire_DWork.Delay_DSTATE_j5;
  u1 = FlyByWire_P.Constant_Value_b * FlyByWire_U.in.time.dt;
  if (u0_1 < u1) {
    u1 = u0_1;
  }

  rtb_Limiterxi1 = FlyByWire_U.in.time.dt * FlyByWire_P.Constant1_Value_a;
  if (u1 > rtb_Limiterxi1) {
    rtb_Limiterxi1 = u1;
  }

  FlyByWire_DWork.Delay_DSTATE_j5 += rtb_Limiterxi1;
  rtb_Y_o = rtb_BusAssignment_p_roll_data_computed_in_flight_gain + FlyByWire_U.in.data.autopilot_custom_on;
  if (rtb_Y_o > FlyByWire_P.Saturation1_UpperSat_e) {
    rtb_Sum1_a = FlyByWire_P.Saturation1_UpperSat_e;
  } else if (rtb_Y_o < FlyByWire_P.Saturation1_LowerSat_l) {
    rtb_Sum1_a = FlyByWire_P.Saturation1_LowerSat_l;
  } else {
    rtb_Sum1_a = rtb_Y_o;
  }

  if (rtb_Sum1_a > FlyByWire_P.Saturation_UpperSat_ll) {
    rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_ll;
  } else {
    if (rtb_Sum1_a < FlyByWire_P.Saturation_LowerSat_og) {
      rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_og;
    }
  }

  rtb_LimiteriH = rtb_BusAssignment_e_roll_law_normal_xi_deg * rtb_Sum1_a;
  rtb_Sum1_a = FlyByWire_P.Constant_Value_l1 - rtb_Sum1_a;
  rtb_Sum1_a *= rtb_BusAssignment_p_roll_data_computed_delta_xi_deg;
  rtb_LimiteriH += rtb_Sum1_a;
  if (rtb_Y_o > FlyByWire_P.Saturation_UpperSat_eq) {
    rtb_Y_o = FlyByWire_P.Saturation_UpperSat_eq;
  } else {
    if (rtb_Y_o < FlyByWire_P.Saturation_LowerSat_n) {
      rtb_Y_o = FlyByWire_P.Saturation_LowerSat_n;
    }
  }

  if (rtb_Y_o > FlyByWire_P.Saturation_UpperSat_i) {
    rtb_Sum1_a = FlyByWire_P.Saturation_UpperSat_i;
  } else if (rtb_Y_o < FlyByWire_P.Saturation_LowerSat_f) {
    rtb_Sum1_a = FlyByWire_P.Saturation_LowerSat_f;
  } else {
    rtb_Sum1_a = rtb_Y_o;
  }

  rtb_Limiterxi = rtb_Y_e * rtb_Sum1_a;
  rtb_Sum1_a = FlyByWire_P.Constant_Value_f - rtb_Sum1_a;
  rtb_Sum1_a *= rtb_Gain1_mp;
  rtb_Limiterxi += rtb_Sum1_a;
  rtb_Y_o = rtb_Limiterxi;
  FlyByWire_RateLimiter(rtb_BusAssignment_m_pitch_output_eta_deg, FlyByWire_P.RateLimitereta_up,
                        FlyByWire_P.RateLimitereta_lo, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimitereta_InitialCondition, &rtb_Sum1_a, &FlyByWire_DWork.sf_RateLimiter_mi);
  rtb_Sum1_a *= FlyByWire_P.Gaineta_Gain_d;
  if (rtb_Sum1_a > FlyByWire_P.Limitereta_UpperSat) {
    FlyByWire_Y.out.output.eta_pos = FlyByWire_P.Limitereta_UpperSat;
  } else if (rtb_Sum1_a < FlyByWire_P.Limitereta_LowerSat) {
    FlyByWire_Y.out.output.eta_pos = FlyByWire_P.Limitereta_LowerSat;
  } else {
    FlyByWire_Y.out.output.eta_pos = rtb_Sum1_a;
  }

  FlyByWire_RateLimiter(rtb_LimiteriH, FlyByWire_P.RateLimiterxi_up, FlyByWire_P.RateLimiterxi_lo,
                        FlyByWire_U.in.time.dt, FlyByWire_P.RateLimiterxi_InitialCondition, &rtb_Limiterxi,
                        &FlyByWire_DWork.sf_RateLimiter_h);
  rtb_Limiterxi *= FlyByWire_P.Gainxi_Gain_n;
  if (rtb_Limiterxi > FlyByWire_P.Limiterxi_UpperSat) {
    FlyByWire_Y.out.output.xi_pos = FlyByWire_P.Limiterxi_UpperSat;
  } else if (rtb_Limiterxi < FlyByWire_P.Limiterxi_LowerSat) {
    FlyByWire_Y.out.output.xi_pos = FlyByWire_P.Limiterxi_LowerSat;
  } else {
    FlyByWire_Y.out.output.xi_pos = rtb_Limiterxi;
  }

  FlyByWire_RateLimiter(rtb_Y_o, FlyByWire_P.RateLimiterzeta_up, FlyByWire_P.RateLimiterzeta_lo, FlyByWire_U.in.time.dt,
                        FlyByWire_P.RateLimiterzeta_InitialCondition, &rtb_Limiterxi1,
                        &FlyByWire_DWork.sf_RateLimiter_d0);
  rtb_Limiterxi1 *= FlyByWire_P.Gainxi1_Gain_e;
  if (rtb_Limiterxi1 > FlyByWire_P.Limiterxi1_UpperSat) {
    FlyByWire_Y.out.output.zeta_pos = FlyByWire_P.Limiterxi1_UpperSat;
  } else if (rtb_Limiterxi1 < FlyByWire_P.Limiterxi1_LowerSat) {
    FlyByWire_Y.out.output.zeta_pos = FlyByWire_P.Limiterxi1_LowerSat;
  } else {
    FlyByWire_Y.out.output.zeta_pos = rtb_Limiterxi1;
  }

  FlyByWire_Y.out.sim.time.dt = FlyByWire_U.in.time.dt;
  FlyByWire_Y.out.sim.time.simulation_time = FlyByWire_U.in.time.simulation_time;
  FlyByWire_Y.out.sim.time.monotonic_time = FlyByWire_DWork.Delay_DSTATE;
  FlyByWire_Y.out.sim.data.nz_g = FlyByWire_U.in.data.nz_g;
  FlyByWire_Y.out.sim.data.Theta_deg = rtb_GainTheta;
  FlyByWire_Y.out.sim.data.Phi_deg = rtb_GainPhi;
  FlyByWire_Y.out.sim.data.q_deg_s = rtb_Gainqk;
  FlyByWire_Y.out.sim.data.r_deg_s = rtb_Gain;
  FlyByWire_Y.out.sim.data.p_deg_s = rtb_Gainpk;
  FlyByWire_Y.out.sim.data.pk_deg_s = rtb_BusAssignment_a_sim_data_pk_deg_s;
  FlyByWire_Y.out.sim.data.qk_dot_deg_s2 = rtb_BusAssignment_a_sim_data_qk_dot_deg_s2;
  FlyByWire_Y.out.sim.data.psi_magnetic_deg = FlyByWire_U.in.data.psi_magnetic_deg;
  FlyByWire_Y.out.sim.data.psi_true_deg = FlyByWire_U.in.data.psi_true_deg;
  FlyByWire_Y.out.sim.data.eta_deg = rtb_Gainpk4;
  FlyByWire_Y.out.sim.data.eta_trim_deg = rtb_Gainpk2;
  FlyByWire_Y.out.sim.data.zeta_trim_deg = rtb_BusAssignment_sim_data_zeta_trim_deg;
  FlyByWire_Y.out.sim.data.alpha_deg = FlyByWire_U.in.data.alpha_deg;
  FlyByWire_Y.out.sim.data.beta_deg = FlyByWire_U.in.data.beta_deg;
  FlyByWire_Y.out.sim.data.beta_dot_deg_s = FlyByWire_U.in.data.beta_dot_deg_s;
  FlyByWire_Y.out.sim.data.V_ias_kn = FlyByWire_U.in.data.V_ias_kn;
  FlyByWire_Y.out.sim.data.V_tas_kn = FlyByWire_U.in.data.V_tas_kn;
  FlyByWire_Y.out.sim.data.V_mach = FlyByWire_U.in.data.V_mach;
  FlyByWire_Y.out.sim.data.H_ft = FlyByWire_U.in.data.H_ft;
  FlyByWire_Y.out.sim.data.H_ind_ft = FlyByWire_U.in.data.H_ind_ft;
  FlyByWire_Y.out.sim.data.H_radio_ft = FlyByWire_U.in.data.H_radio_ft;
  FlyByWire_Y.out.sim.data.CG_percent_MAC = FlyByWire_U.in.data.CG_percent_MAC;
  FlyByWire_Y.out.sim.data.total_weight_kg = FlyByWire_U.in.data.total_weight_kg;
  u0_1 = FlyByWire_P.Gain_Gain_i * FlyByWire_U.in.data.gear_animation_pos_0 - FlyByWire_P.Constant_Value_g;
  if (u0_1 > FlyByWire_P.Saturation_UpperSat_e) {
    FlyByWire_Y.out.sim.data.gear_strut_compression_0 = FlyByWire_P.Saturation_UpperSat_e;
  } else if (u0_1 < FlyByWire_P.Saturation_LowerSat_e) {
    FlyByWire_Y.out.sim.data.gear_strut_compression_0 = FlyByWire_P.Saturation_LowerSat_e;
  } else {
    FlyByWire_Y.out.sim.data.gear_strut_compression_0 = u0_1;
  }

  FlyByWire_Y.out.sim.data.gear_strut_compression_1 = u0;
  FlyByWire_Y.out.sim.data.gear_strut_compression_2 = u0_0;
  FlyByWire_Y.out.sim.data.flaps_handle_index = FlyByWire_U.in.data.flaps_handle_index;
  FlyByWire_Y.out.sim.data.spoilers_left_pos = FlyByWire_U.in.data.spoilers_left_pos;
  FlyByWire_Y.out.sim.data.spoilers_right_pos = FlyByWire_U.in.data.spoilers_right_pos;
  FlyByWire_Y.out.sim.data.autopilot_master_on = FlyByWire_U.in.data.autopilot_master_on;
  FlyByWire_Y.out.sim.data.slew_on = FlyByWire_U.in.data.slew_on;
  FlyByWire_Y.out.sim.data.pause_on = FlyByWire_U.in.data.pause_on;
  FlyByWire_Y.out.sim.data.tracking_mode_on_override = FlyByWire_U.in.data.tracking_mode_on_override;
  FlyByWire_Y.out.sim.data.autopilot_custom_on = FlyByWire_U.in.data.autopilot_custom_on;
  FlyByWire_Y.out.sim.data.autopilot_custom_Theta_c_deg = FlyByWire_U.in.data.autopilot_custom_Theta_c_deg;
  FlyByWire_Y.out.sim.data.autopilot_custom_Phi_c_deg = FlyByWire_U.in.data.autopilot_custom_Phi_c_deg;
  FlyByWire_Y.out.sim.data.autopilot_custom_Beta_c_deg = FlyByWire_U.in.data.autopilot_custom_Beta_c_deg;
  FlyByWire_Y.out.sim.data.simulation_rate = FlyByWire_U.in.data.simulation_rate;
  FlyByWire_Y.out.sim.data.ice_structure_percent = FlyByWire_U.in.data.ice_structure_percent;
  FlyByWire_Y.out.sim.data.linear_cl_alpha_per_deg = FlyByWire_U.in.data.linear_cl_alpha_per_deg;
  FlyByWire_Y.out.sim.data.alpha_stall_deg = FlyByWire_U.in.data.alpha_stall_deg;
  FlyByWire_Y.out.sim.data.alpha_zero_lift_deg = FlyByWire_U.in.data.alpha_zero_lift_deg;
  FlyByWire_Y.out.sim.data.ambient_density_kg_per_m3 = FlyByWire_U.in.data.ambient_density_kg_per_m3;
  FlyByWire_Y.out.sim.data.ambient_pressure_mbar = FlyByWire_U.in.data.ambient_pressure_mbar;
  FlyByWire_Y.out.sim.data.ambient_temperature_celsius = FlyByWire_U.in.data.ambient_temperature_celsius;
  FlyByWire_Y.out.sim.data.ambient_wind_x_kn = FlyByWire_U.in.data.ambient_wind_x_kn;
  FlyByWire_Y.out.sim.data.ambient_wind_y_kn = FlyByWire_U.in.data.ambient_wind_y_kn;
  FlyByWire_Y.out.sim.data.ambient_wind_z_kn = FlyByWire_U.in.data.ambient_wind_z_kn;
  FlyByWire_Y.out.sim.data.ambient_wind_velocity_kn = FlyByWire_U.in.data.ambient_wind_velocity_kn;
  FlyByWire_Y.out.sim.data.ambient_wind_direction_deg = FlyByWire_U.in.data.ambient_wind_direction_deg;
  FlyByWire_Y.out.sim.data.total_air_temperature_celsius = FlyByWire_U.in.data.total_air_temperature_celsius;
  FlyByWire_Y.out.sim.data.latitude_deg = FlyByWire_U.in.data.latitude_deg;
  FlyByWire_Y.out.sim.data.longitude_deg = FlyByWire_U.in.data.longitude_deg;
  FlyByWire_Y.out.sim.data.engine_1_thrust_lbf = FlyByWire_U.in.data.engine_1_thrust_lbf;
  FlyByWire_Y.out.sim.data.engine_2_thrust_lbf = FlyByWire_U.in.data.engine_2_thrust_lbf;
  FlyByWire_Y.out.sim.data.thrust_lever_1_pos = FlyByWire_U.in.data.thrust_lever_1_pos;
  FlyByWire_Y.out.sim.data.thrust_lever_2_pos = FlyByWire_U.in.data.thrust_lever_2_pos;
  FlyByWire_Y.out.sim.data.tailstrike_protection_on = FlyByWire_U.in.data.tailstrike_protection_on;
  FlyByWire_Y.out.sim.data.VLS_kn = FlyByWire_U.in.data.VLS_kn;
  FlyByWire_Y.out.sim.data_computed.on_ground = rtb_on_ground;
  FlyByWire_Y.out.sim.data_computed.tracking_mode_on = rtb_alpha_floor_inhib;
  FlyByWire_Y.out.sim.data_computed.alpha_floor_command = FlyByWire_DWork.sAlphaFloor;
  FlyByWire_Y.out.sim.data_computed.protection_ap_disc = rtb_NOT2;
  FlyByWire_Y.out.sim.data_computed.high_speed_prot_active = rtb_ap_special_disc;
  FlyByWire_Y.out.sim.data_computed.high_speed_prot_high_kn = rtb_Min5;
  FlyByWire_Y.out.sim.data_speeds_aoa.alpha_max_deg = rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_max_deg;
  FlyByWire_Y.out.sim.data_speeds_aoa.alpha_prot_deg = rtb_Switch;
  FlyByWire_Y.out.sim.data_speeds_aoa.alpha_floor_deg = rtb_BusAssignment_c_sim_data_speeds_aoa_alpha_floor_deg;
  FlyByWire_Y.out.sim.input.delta_eta_pos = rtb_BusAssignment_sim_input_delta_eta_pos;
  FlyByWire_Y.out.sim.input.delta_xi_pos = rtb_BusAssignment_sim_input_delta_xi_pos;
  FlyByWire_Y.out.sim.input.delta_zeta_pos = rtb_BusAssignment_sim_input_delta_zeta_pos;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_limit_lo =
    rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_lo;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_limit_up =
    rtb_BusAssignment_cs_pitch_data_computed_eta_trim_deg_limit_up;
  FlyByWire_Y.out.pitch.data_computed.delta_eta_deg = rtb_BusAssignment_cs_pitch_data_computed_delta_eta_deg;
  FlyByWire_Y.out.pitch.data_computed.in_flight = FlyByWire_B.in_flight;
  FlyByWire_Y.out.pitch.data_computed.in_rotation = rtb_in_rotation;
  FlyByWire_Y.out.pitch.data_computed.in_flare = rtb_in_flare;
  FlyByWire_Y.out.pitch.data_computed.in_flight_gain = rtb_BusAssignment_cs_pitch_data_computed_in_flight_gain;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_should_freeze = rtb_eta_trim_deg_should_freeze;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_reset = rtb_eta_trim_deg_reset;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_reset_deg = rtb_nz_limit_up_g;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_should_write = rtb_eta_trim_deg_should_write;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_rate_limit_up_deg_s = rtb_ManualSwitch;
  FlyByWire_Y.out.pitch.data_computed.eta_trim_deg_rate_limit_lo_deg_s = rtb_eta_trim_deg_rate_limit_lo_deg_s;
  FlyByWire_Y.out.pitch.data_computed.flare_Theta_c_deg = FlyByWire_DWork.Delay_DSTATE_dq;
  FlyByWire_Y.out.pitch.data_computed.flare_Theta_c_rate_deg_s = FlyByWire_B.flare_Theta_c_rate_deg_s;
  FlyByWire_Y.out.pitch.law_rotation.eta_deg = rtb_BusAssignment_cs_sim_data_qk_deg_s;
  FlyByWire_Y.out.pitch.law_normal.protection_alpha_c_deg = rtb_Switch + rtb_y_g;
  FlyByWire_Y.out.pitch.law_normal.protection_V_c_kn = rtb_v_target;
  FlyByWire_Y.out.pitch.law_normal.eta_dot_deg_s = rtb_BusAssignment_f_pitch_law_normal_eta_dot_deg_s;
  FlyByWire_Y.out.pitch.vote.eta_dot_deg_s = rtb_BusAssignment_f_pitch_law_normal_eta_dot_deg_s;
  FlyByWire_Y.out.pitch.integrated.eta_deg = FlyByWire_DWork.Delay_DSTATE_f1;
  FlyByWire_Y.out.pitch.output.eta_deg = rtb_BusAssignment_m_pitch_output_eta_deg;
  FlyByWire_Y.out.pitch.output.eta_trim_deg = FlyByWire_DWork.Delay_DSTATE_ea;
  FlyByWire_Y.out.roll.data_computed.delta_xi_deg = rtb_BusAssignment_p_roll_data_computed_delta_xi_deg;
  FlyByWire_Y.out.roll.data_computed.delta_zeta_deg = rtb_Gain1_mp;
  FlyByWire_Y.out.roll.data_computed.in_flight = rtb_nz_limit_lo_g;
  FlyByWire_Y.out.roll.data_computed.in_flight_gain = rtb_BusAssignment_p_roll_data_computed_in_flight_gain;
  FlyByWire_Y.out.roll.data_computed.zeta_trim_deg_should_write = (FlyByWire_U.in.data.autopilot_custom_on != 0.0);
  FlyByWire_Y.out.roll.data_computed.beta_target_deg = rtb_Product_e;
  FlyByWire_Y.out.roll.law_normal.xi_deg = rtb_BusAssignment_e_roll_law_normal_xi_deg;
  FlyByWire_Y.out.roll.law_normal.zeta_deg = rtb_Y_e;
  FlyByWire_Y.out.roll.output.xi_deg = rtb_LimiteriH;
  FlyByWire_Y.out.roll.output.zeta_deg = rtb_Y_o;
  FlyByWire_Y.out.roll.output.zeta_trim_deg = FlyByWire_DWork.Delay_DSTATE_j5;
  u0 = FlyByWire_P.GainiH_Gain * FlyByWire_DWork.Delay_DSTATE_ea;
  if (u0 > FlyByWire_P.LimiteriH_UpperSat) {
    FlyByWire_Y.out.output.eta_trim_deg = FlyByWire_P.LimiteriH_UpperSat;
  } else if (u0 < FlyByWire_P.LimiteriH_LowerSat) {
    FlyByWire_Y.out.output.eta_trim_deg = FlyByWire_P.LimiteriH_LowerSat;
  } else {
    FlyByWire_Y.out.output.eta_trim_deg = u0;
  }

  FlyByWire_Y.out.output.eta_trim_deg_should_write = rtb_eta_trim_deg_should_write;
  u0 = FlyByWire_P.Gainxi2_Gain * FlyByWire_DWork.Delay_DSTATE_j5;
  if (u0 > FlyByWire_P.Limiterxi2_UpperSat) {
    FlyByWire_Y.out.output.zeta_trim_pos = FlyByWire_P.Limiterxi2_UpperSat;
  } else if (u0 < FlyByWire_P.Limiterxi2_LowerSat) {
    FlyByWire_Y.out.output.zeta_trim_pos = FlyByWire_P.Limiterxi2_LowerSat;
  } else {
    FlyByWire_Y.out.output.zeta_trim_pos = u0;
  }

  FlyByWire_Y.out.output.zeta_trim_pos_should_write = (FlyByWire_U.in.data.autopilot_custom_on != 0.0);
  FlyByWire_DWork.Delay_DSTATE_d = rtb_Gain_kh;
  FlyByWire_DWork.Delay_DSTATE_j = rtb_Gain_g;
  FlyByWire_DWork.Delay_DSTATE_c = rtb_Divide_m;
  FlyByWire_DWork.Delay_DSTATE_p = rtb_Gain_c;
  FlyByWire_DWork.Delay_DSTATE_m = rtb_Divide_e;
  FlyByWire_DWork.Delay_DSTATE_ca = rtb_Y;
  FlyByWire_DWork.Delay_DSTATE_jv = rtb_Y_k;
  FlyByWire_DWork.Delay_DSTATE_ps = rtb_Y_h;
  FlyByWire_DWork.Delay_DSTATE_c1 = rtb_Divide1;
  FlyByWire_DWork.Delay_DSTATE_l = rtb_Sum_l;
  FlyByWire_DWork.Delay_DSTATE_n = rtb_Divide_b;
  FlyByWire_DWork.Delay_DSTATE_f = rtb_Limiterxi2;
  FlyByWire_DWork.Delay_DSTATE_dd = rtb_Product1_n5;
  FlyByWire_DWork.icLoad = 0U;
  FlyByWire_DWork.icLoad_e = 0U;
  FlyByWire_DWork.icLoad_i = 0U;
  FlyByWire_DWork.icLoad_m = 0U;
  FlyByWire_DWork.icLoad_id = 0U;
  FlyByWire_DWork.icLoad_c = 0U;
}

void FlyByWireModelClass::initialize()
{
  FlyByWire_DWork.Delay_DSTATE = FlyByWire_P.Delay_InitialCondition;
  FlyByWire_DWork.Memory_PreviousInput = FlyByWire_P.SRFlipFlop_initial_condition;
  FlyByWire_DWork.Delay_DSTATE_d = FlyByWire_P.DiscreteDerivativeVariableTs_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_dq = FlyByWire_P.RateLimiterDynamicVariableTs_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_i = FlyByWire_P.RateLimiterVariableTs3_InitialCondition_e;
  FlyByWire_DWork.Delay_DSTATE_j = FlyByWire_P.DiscreteDerivativeVariableTs2_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_c = FlyByWire_P.Delay_InitialCondition_j;
  FlyByWire_DWork.Delay1_DSTATE = FlyByWire_P.Delay1_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_p = FlyByWire_P.DiscreteDerivativeVariableTs2_InitialCondition_c;
  FlyByWire_DWork.Delay_DSTATE_m = FlyByWire_P.Delay_InitialCondition_l;
  FlyByWire_DWork.Delay1_DSTATE_i = FlyByWire_P.Delay1_InitialCondition_a;
  FlyByWire_DWork.Delay_DSTATE_g = FlyByWire_P.RateLimiterVariableTs4_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_ca = FlyByWire_P.DiscreteDerivativeVariableTs1_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_jv = FlyByWire_P.DiscreteDerivativeVariableTs_InitialCondition_c;
  FlyByWire_DWork.Delay_DSTATE_k = FlyByWire_P.RateLimiterVariableTs5_InitialCondition;
  FlyByWire_DWork.Delay_DSTATE_ps = FlyByWire_P.DiscreteDerivativeVariableTs2_InitialCondition_cx;
  FlyByWire_DWork.Delay_DSTATE_c1 = FlyByWire_P.Delay_InitialCondition_k;
  FlyByWire_DWork.Delay1_DSTATE_o = FlyByWire_P.Delay1_InitialCondition_i;
  FlyByWire_DWork.Delay_DSTATE_l = FlyByWire_P.DiscreteDerivativeVariableTs2_InitialCondition_b;
  FlyByWire_DWork.Delay_DSTATE_n = FlyByWire_P.Delay_InitialCondition_p;
  FlyByWire_DWork.Delay1_DSTATE_n = FlyByWire_P.Delay1_InitialCondition_k;
  FlyByWire_DWork.Delay_DSTATE_f = FlyByWire_P.DiscreteDerivativeVariableTs_InitialCondition_d;
  FlyByWire_DWork.Delay_DSTATE_dd = FlyByWire_P.DiscreteDerivativeVariableTs_InitialCondition_k;
  FlyByWire_DWork.icLoad = 1U;
  FlyByWire_DWork.icLoad_e = 1U;
  FlyByWire_DWork.icLoad_i = 1U;
  FlyByWire_DWork.Delay_DSTATE_ea = FlyByWire_P.RateLimiterDynamicVariableTs_InitialCondition_i;
  FlyByWire_DWork.icLoad_m = 1U;
  FlyByWire_DWork.icLoad_id = 1U;
  FlyByWire_DWork.Delay_DSTATE_j5 = FlyByWire_P.RateLimiterDynamicVariableTs_InitialCondition_i4;
  FlyByWire_DWork.icLoad_c = 1U;
}

void FlyByWireModelClass::terminate()
{
}

FlyByWireModelClass::FlyByWireModelClass() :
  FlyByWire_B(),
  FlyByWire_DWork(),
  FlyByWire_U(),
  FlyByWire_Y()
{
}

FlyByWireModelClass::~FlyByWireModelClass()
{
}