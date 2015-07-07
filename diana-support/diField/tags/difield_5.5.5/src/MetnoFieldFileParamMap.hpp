#ifndef METNOFIELDFILEPARAMMAP_
#define METNOFIELDFILEPARAMMAP_

#include <map>

/**
 * Fills two maps, one with a mapping between met.no fieldfiles indices to
 * names, the other one maps the same indices to units.
 * @param fieldfileParamMap Mapping of fieldfile indices to names
 * @param fieldfileUnitMap Mapping of fieldfile indices to units
 */
static void fillMaps(std::map<int, std::string>& fieldfileParamMap, std::map<int, std::string>& fieldfileUnitMap) __attribute__((__noinline__));
static void fillMaps(std::map<int, std::string>& fieldfileParamMap, std::map<int, std::string>& fieldfileUnitMap) {

fieldfileParamMap[1] = "Z_geopotensiell"; fieldfileUnitMap[1] = "meter";
fieldfileParamMap[2] = "u_vind"; fieldfileUnitMap[2] = "m/s";
fieldfileParamMap[3] = "v_vind"; fieldfileUnitMap[3] = "m/s";
fieldfileParamMap[4] = "T_absolutt_temperatur"; fieldfileUnitMap[4] = "K";
fieldfileParamMap[5] = "Z_tykkelse_geopot"; fieldfileUnitMap[5] = "m";
fieldfileParamMap[6] = "dd_vindretning_WMO_grade"; fieldfileUnitMap[6] = "1";
fieldfileParamMap[7] = "ff_vindstyrke"; fieldfileUnitMap[7] = "knop";
fieldfileParamMap[8] = "p_lufttrykk"; fieldfileUnitMap[8] = "hPa";
fieldfileParamMap[9] = "q_spesifikk_fuktighet"; fieldfileUnitMap[9] = "1";
fieldfileParamMap[10] = "r_relativ_fuktighet"; fieldfileUnitMap[10] = "percent";
fieldfileParamMap[11] = "sigma_dot";  fieldfileUnitMap[11] = "1";
fieldfileParamMap[12] = "w_vertikal_hastighet"; fieldfileUnitMap[12] = "m/s";
fieldfileParamMap[13] = "omega_vertikal_hastighet"; fieldfileUnitMap[13] = "mb/s";
fieldfileParamMap[14] = "Rt_nedboer_intensitet"; fieldfileUnitMap[14] = "mm/h";
fieldfileParamMap[15] = "RR1_frontal_nedboer_akkumulert"; fieldfileUnitMap[15] = "mm";
fieldfileParamMap[16] = "RR2_konvektiv_nedboer_hver_3"; fieldfileUnitMap[16] = "mm";
fieldfileParamMap[17] = "Total_nedboer_akk_fra_0"; fieldfileUnitMap[17] = "mm";
fieldfileParamMap[18] = "theta_potensiell_temp"; fieldfileUnitMap[18] = "K";
fieldfileParamMap[19] = "RR1_frontal_nedboer_akk"; fieldfileUnitMap[19] = "mm";
fieldfileParamMap[20] = "RR2_konvektiv_nedboer_fra_0"; fieldfileUnitMap[20] = "mm";
fieldfileParamMap[21] = "dPs_over_dt";  fieldfileUnitMap[21] = "1";
fieldfileParamMap[22] = "qliq_skyvann";  fieldfileUnitMap[22] = "1";
fieldfileParamMap[23] = "precip_nedboer_i_modell_flater"; fieldfileUnitMap[23] = "mm";
fieldfileParamMap[24] = "snowfall"; fieldfileUnitMap[24] = "m";
fieldfileParamMap[25] = "total_sky_dekke"; fieldfileUnitMap[25] = "percent";

fieldfileParamMap[27] = "skyvann_utfellbart_sigma_gt_0.85_middel_6_til_8_timer"; fieldfileUnitMap[27] = "mm";
fieldfileParamMap[28] = "skyvann_utfellbart_sigma_lt_0.85_middel_6_til_8_timer"; fieldfileUnitMap[28] = "mm";
fieldfileParamMap[29] = "Tsoil_temperatur_under_bakken"; fieldfileUnitMap[29] = "K";
fieldfileParamMap[30] = "T0_temperatur_i_0m_hoeyde"; fieldfileUnitMap[30] = "K";
fieldfileParamMap[31] = "T2_temperatur_i_2m_hoeyde"; fieldfileUnitMap[31] = "K";
fieldfileParamMap[32] = "relativ_fuktighet_i_2m_hoeyde"; fieldfileUnitMap[32] = "percent";
fieldfileParamMap[33] = "u_vind_komponent_i_x_retn_i_10m_hoeyde"; fieldfileUnitMap[33] = "1";
fieldfileParamMap[34] = "v_vind_komponent_i_y_retn_i_10m_hoeyde"; fieldfileUnitMap[34] = "1";
fieldfileParamMap[35] = "global_straaling_accum"; fieldfileUnitMap[35] = "kJ/m^2";
fieldfileParamMap[36] = "accum_surface_flux_of_sensibel_heat"; fieldfileUnitMap[36] = "kJ/m^2";
fieldfileParamMap[37] = "accum_surface_flux_of_latent_heat"; fieldfileUnitMap[37] = "kJ/m^2";
fieldfileParamMap[38] = "surface_stress"; fieldfileUnitMap[38] = "N/m^2";
fieldfileParamMap[39] = "skydekke"; fieldfileUnitMap[39] = "percent";
/*	      ( 188)	          nivaa1= 300 : h�ye
	      ( 187)		  nivaa1= 500 : middels h�ye
              ( 186)		  nivaa1= 850 : lave
	      (    )	          nivaa1=1000 : t�ke
				  nivaa1< 300 : i modell-flater*/
fieldfileParamMap[40] = "RR1_frontal_nedboer_akkum_6_til_8"; fieldfileUnitMap[40] = "mm";
fieldfileParamMap[41] = "RR2_konvektiv_nedboer_akkum_6_til_8"; fieldfileUnitMap[41] = "mm";
fieldfileParamMap[42] = "Vertikalt_integrert_vanndamp"; fieldfileUnitMap[42] = "1";
fieldfileParamMap[43] = "konvektivt_areal_middel_6_til_8t"; fieldfileUnitMap[43] = "percent";
fieldfileParamMap[44] = "konveksjons_topp_max_6_til_8t"; fieldfileUnitMap[44] = "mb";
fieldfileParamMap[45] = "konveksjons_base_min_6_til_8t"; fieldfileUnitMap[45] = "mb";
fieldfileParamMap[46] = "global_straaling_UM_1hr_accum"; fieldfileUnitMap[46] = "1";
fieldfileParamMap[47] = "skyvann_tetthet_middel_6_til_8t"; fieldfileUnitMap[47] = "kg/m^3";
fieldfileParamMap[48] = "vanndamp_spesifikk_fuktighet_for_sigma_0.925"; fieldfileUnitMap[48] = "1";
fieldfileParamMap[49] = "potential_evaporation_accum"; fieldfileUnitMap[49] = "J/m^2";
fieldfileParamMap[50] = "W_surface_soil_water_content"; fieldfileUnitMap[50] = "percent";
fieldfileParamMap[51] = "WB_bulk_soil_water_content"; fieldfileUnitMap[51] = "m";
fieldfileParamMap[52] = "WR_intercepted_water_content"; fieldfileUnitMap[52] = "1";
fieldfileParamMap[53] = "Friction_velocity"; fieldfileUnitMap[52] = "m/s";
fieldfileParamMap[54] = "Friction_temperature"; fieldfileUnitMap[53] = "K";
fieldfileParamMap[55] = "Friction_humidity"; fieldfileUnitMap[55] = "1";
fieldfileParamMap[56] = "Surface_Air_Density"; fieldfileUnitMap[56] = "kg/m^3";
fieldfileParamMap[57] = "Richardson_number"; fieldfileUnitMap[57] = "1";
fieldfileParamMap[58] = "Mean_Sea_Level_pressure"; fieldfileUnitMap[58] = "hPa";
fieldfileParamMap[59] = "skydekning_for_sigma_gt_0.85_middel_6_til_8t"; fieldfileUnitMap[59] = "percent";
fieldfileParamMap[60] = "skydekning_for_sigma_lt_0.85"; fieldfileUnitMap[60] = "1";
fieldfileParamMap[61] = "hoeyde_for_smelting_faseovergang"; fieldfileUnitMap[61] = "m";
fieldfileParamMap[62] = "hoeyde_for_nedboer_faseovergang"; fieldfileUnitMap[62] = "m";
fieldfileParamMap[63] = "snoe_nedbor_vannekvivalent"; fieldfileUnitMap[63] = "mm";
fieldfileParamMap[64] = "snoe_stratiform_nedbor_vannekv"; fieldfileUnitMap[64] = "mm";
fieldfileParamMap[65] = "snoe_konvektiv_nedbor_vannekv"; fieldfileUnitMap[65] = "mm";
fieldfileParamMap[66] = "snoe_bakke_vannekvivalent"; fieldfileUnitMap[66] = "m";

fieldfileParamMap[68] = "surface_flux_of_sensibel_heat"; fieldfileUnitMap[68] = "W/m^2";
fieldfileParamMap[69] = "surface_flux_of_latent_heat"; fieldfileUnitMap[69] = "W/m^2";
fieldfileParamMap[70] = "spesial_beregnet_nedboer_akkum"; fieldfileUnitMap[70] = "mm";
fieldfileParamMap[71] = "modell_nedboer_for_plotting"; fieldfileUnitMap[71] = "mm";
fieldfileParamMap[72] = "x_component_of_surface_stress"; fieldfileUnitMap[72] = "N/m^2";
fieldfileParamMap[73] = "y-component_of_surface_stress"; fieldfileUnitMap[73] = "N/m^2";
fieldfileParamMap[74] = "x_comp_of_friction_velocity"; fieldfileUnitMap[74] = "m/s";
fieldfileParamMap[75] = "y_comp_of_friction_velocity"; fieldfileUnitMap[75] = "m/s";
fieldfileParamMap[76] = "x_comp_of_geostrophic_wind"; fieldfileUnitMap[76] = "m/s";
fieldfileParamMap[77] = "y_comp_of_geostrophic_wind"; fieldfileUnitMap[77] = "m/s";
fieldfileParamMap[78] = "accum_downward_longwave_radiation"; fieldfileUnitMap[78] = "kJ/m^2";
fieldfileParamMap[79] = "accum_upward_longwave_radiation"; fieldfileUnitMap[79] = "kJ/m^2";
fieldfileParamMap[80] = "Potential_Vorticity"; fieldfileUnitMap[80] = "pvu";
fieldfileParamMap[81] = "Ducting"; fieldfileUnitMap[81] = "1";
fieldfileParamMap[82] = "Spesific_humidity_at_2_meter"; fieldfileUnitMap[82] = "kg/kg";
fieldfileParamMap[83] = "Roughness"; fieldfileUnitMap[83] = "m";

fieldfileParamMap[85] = "Soil_water_content"; fieldfileUnitMap[85] = "m";
fieldfileParamMap[86] = "Deep_soil_water_content"; fieldfileUnitMap[86] = "m";
fieldfileParamMap[87] = "surface_longwave_radiation"; fieldfileUnitMap[87] = "KW/m^2s";
fieldfileParamMap[88] = "Vorticity_relative"; fieldfileUnitMap[88] = "s^-1";

fieldfileParamMap[90] = "Montgomery_potential"; fieldfileUnitMap[90] = "m^2/s^2";
fieldfileParamMap[91] = "Clear_Air_Turbulence_CAT"; fieldfileUnitMap[91] = "1";
fieldfileParamMap[92] = "Spec_hum_icing_danger"; fieldfileUnitMap[92] = "1";
fieldfileParamMap[93] = "Convective_Available_Potential_Energy_CAPE"; fieldfileUnitMap[93] = "1";
fieldfileParamMap[94] = "Turbulensintensitet_spesialflate"; fieldfileUnitMap[94] = "m/s";

fieldfileParamMap[97] = "Hoyde_av_blandingslaget_abl"; fieldfileUnitMap[97] = "m";
fieldfileParamMap[98] = "Hoyde_til_laveste_konvek_sky"; fieldfileUnitMap[98] = "m";
fieldfileParamMap[99] = "Temp_Diff_10m_2m"; fieldfileUnitMap[99] = "K";

fieldfileParamMap[101] = "Zs_topografi"; fieldfileUnitMap[101] = "m";
fieldfileParamMap[102] = "Znull_0_hav_1_is_2_land)"; fieldfileUnitMap[102] = "1";
fieldfileParamMap[103] = "SST_sjoetemperatur_unit_is_C_or_K"; fieldfileUnitMap[103] = "degrees";
fieldfileParamMap[104] = "snoedekke"; fieldfileUnitMap[104] = "percent";
fieldfileParamMap[105] = "snoedybde"; fieldfileUnitMap[105] = "cm";
fieldfileParamMap[106] = "snow_density"; fieldfileUnitMap[106] = "kg/m^3";
fieldfileParamMap[107] = "snow_evaporation_water_eqv"; fieldfileUnitMap[107] = "m";
fieldfileParamMap[108] = "snow_melt_water_eqv"; fieldfileUnitMap[108] = "m";
fieldfileParamMap[109] = "temp_of_snow_layer"; fieldfileUnitMap[109] = "K";
fieldfileParamMap[110] = "Nedb�rsannsynlighet_EPS"; fieldfileUnitMap[110] = "percent";
fieldfileParamMap[111] = "Vindsannsynlighet_EPS"; fieldfileUnitMap[111] = "percent";
fieldfileParamMap[112] = "Temp_anomali_i_850hPa_EPS"; fieldfileUnitMap[112] = "percent";
fieldfileParamMap[113] = "Sanns_nedboer_mm_over_6h"; fieldfileUnitMap[113] = "percent";
fieldfileParamMap[114] = "Sanns_T2m_K"; fieldfileUnitMap[114] = "percent";
fieldfileParamMap[115] = "Sanns_vind_10m_m_over_s)"; fieldfileUnitMap[115] = "percent	";
fieldfileParamMap[116] = "Sanns_T0m_T500hPa_K"; fieldfileUnitMap[116] = "percent";

fieldfileParamMap[120] = "Nedboersannsynlighet_EPS_mm_over_6h"; fieldfileUnitMap[120] = "percent";
fieldfileParamMap[121] = "Max_T2M_during_prev_6_hours_MX2T6"; fieldfileUnitMap[121] = "1";
fieldfileParamMap[122] = "Min_T2M_during_prev_6_hours_MN2T6"; fieldfileUnitMap[122] = "1";
fieldfileParamMap[123] = "Max_ducting"; fieldfileUnitMap[123] = "1";
fieldfileParamMap[124] = "pressure_where_max_duct_occurs"; fieldfileUnitMap[124] = "hPa";
fieldfileParamMap[125] = "height_where_max_duct_occurs"; fieldfileUnitMap[125] = "m";
fieldfileParamMap[126] = "10_metre_wind_gust_in_the_past_6h_10FG6"; fieldfileUnitMap[126] = "m/s";

fieldfileParamMap[131] = "T2m_land_average_Hirlam"; fieldfileUnitMap[131] = "K";

fieldfileParamMap[152] = "Max_u_vind"; fieldfileUnitMap[152] = "m/s";
fieldfileParamMap[153] = "Max_v_vind"; fieldfileUnitMap[153] = "m/s";
fieldfileParamMap[154] = "Max_p_vind"; fieldfileUnitMap[154] = "m/s";
fieldfileParamMap[155] = "Max_turbulensintensitet"; fieldfileUnitMap[155] = "m/s";
fieldfileParamMap[156] = "Hoeyde_av_max_turb_int"; fieldfileUnitMap[156] = "m";

fieldfileParamMap[158] = "Mean_Sea_Level_pressure_1000hPa"; fieldfileUnitMap[158] = "hPa";
fieldfileParamMap[159] = "Trykktendens_3_timer"; fieldfileUnitMap[159] = "hPa";

fieldfileParamMap[160] = "sesong_sannsynlighet"; fieldfileUnitMap[160] = "1";
fieldfileParamMap[161] = "sesong_signifikans"; fieldfileUnitMap[161] = "1";
fieldfileParamMap[162] = "sesong_antall"; fieldfileUnitMap[162] = "1";
fieldfileParamMap[163] = "2m_temperatur_sesong_varsel"; fieldfileUnitMap[163] = "K";

fieldfileParamMap[172] = "Land_sea_mask"; fieldfileUnitMap[172] = "1";

fieldfileParamMap[175] = "albedo_prognostisk"; fieldfileUnitMap[175] = "percent";

fieldfileParamMap[181] = "Fraction_of_land"; fieldfileUnitMap[181] = "percent";
fieldfileParamMap[182] = "Climatological_roughness"; fieldfileUnitMap[182] = "m";
fieldfileParamMap[183] = "Climatological_deep_soil_temp"; fieldfileUnitMap[183] = "Kelvin";
fieldfileParamMap[184] = "Albedo"; fieldfileUnitMap[184] = "percent";
fieldfileParamMap[186] = "Climatological_deep_soil_water"; fieldfileUnitMap[186] = "m";

fieldfileParamMap[191] = "Fraction_of_ice"; fieldfileUnitMap[191] = "percent";

fieldfileParamMap[197] = "Fraction_of_forrest"; fieldfileUnitMap[197] = "percent";

///                 200-299 FOR BOELGEMODELL
fieldfileParamMap[200] = "HS_BOELGEHOEYDE_TOTAL"; fieldfileUnitMap[200] = "m";
fieldfileParamMap[201] = "TP_PEAK_PERIODE_TOTAL"; fieldfileUnitMap[201] = "s";
fieldfileParamMap[202] = "TS_SIGNIFIKANT_PERIODE_TOTAL"; fieldfileUnitMap[202] = "s";
fieldfileParamMap[203] = "DDP_peak_boelgeretning_tot"; fieldfileUnitMap[203] = "degrees";
fieldfileParamMap[204] = "DDM_MEAN_BOELGERETNING_TOT"; fieldfileUnitMap[204] = "degrees";

fieldfileParamMap[210] = "HSWS_boelgehoeyde_vind_sjoe"; fieldfileUnitMap[210] = "m";
fieldfileParamMap[211] = "TPWS_peak_periode_vind_sjoe"; fieldfileUnitMap[211] = "s";
fieldfileParamMap[212] = "DDPWS_peak_boelgeretning_vind"; fieldfileUnitMap[212] = "degrees";
fieldfileParamMap[213] = "TMWS_mean_periode_vind_sjoe"; fieldfileUnitMap[213] = "s";
fieldfileParamMap[214] = "DDMWS_mean_boelgeretning_vind"; fieldfileUnitMap[214] = "degrees";

fieldfileParamMap[220] = "HSSW_boelgehoeyde_doenning"; fieldfileUnitMap[220] = "m";
fieldfileParamMap[221] = "TPSW_peak_periode_doenning"; fieldfileUnitMap[221] = "s";
fieldfileParamMap[222] = "DDPSW_peak_boelgeretning_doenn"; fieldfileUnitMap[222] = "degrees";
fieldfileParamMap[223] = "TMSW_mean_periode_doenning"; fieldfileUnitMap[223] = "s";
fieldfileParamMap[224] = "DDMSW_mean_boelgeretning_doenn"; fieldfileUnitMap[224] = "degrees";

fieldfileParamMap[288] = "MPP_processor_map"; fieldfileUnitMap[288] = "1";

fieldfileParamMap[290] = "bunntopografi"; fieldfileUnitMap[290] = "m";
fieldfileParamMap[291] = "x_komp_av_Stokesdrift"; fieldfileUnitMap[291] = "m/s";
fieldfileParamMap[292] = "y_komp_av_Stokesdrift"; fieldfileUnitMap[292] = "m/s";
fieldfileParamMap[293] = "is_over_ikke_is"; fieldfileUnitMap[293] = "1";

fieldfileParamMap[295] = "TAUWG_Normalized_stress"; fieldfileUnitMap[295] = "1";
fieldfileParamMap[296] = "CDG_Drag_coeffisient"; fieldfileUnitMap[296] = "1";
fieldfileParamMap[297] = "USTARG_Friction_velocity"; fieldfileUnitMap[297] = "1";
fieldfileParamMap[298] = "FF_vind_hastighet"; fieldfileUnitMap[298] = "m/s";
fieldfileParamMap[299] = "DD_vind_retning"; fieldfileUnitMap[299] = "degrees";


///                  300-399 FOR HAVMODELL
fieldfileParamMap[301] = "overflatehevning_over_skilleflater"; fieldfileUnitMap[301] = "m";
fieldfileParamMap[302] = "u_stroem_komponent_i_x_retning"; fieldfileUnitMap[302] = "m/s";
fieldfileParamMap[303] = "v_stroem_komponent_i_y_retning"; fieldfileUnitMap[303] = "m/s";
fieldfileParamMap[304] = "w_stroem_komponent_i_z_retning"; fieldfileUnitMap[304] = "m/s";
fieldfileParamMap[305] = "tethet_sigma_t"; fieldfileUnitMap[305] = "1";
fieldfileParamMap[306] = "lagtykkelse"; fieldfileUnitMap[306] = "m";
fieldfileParamMap[307] = "saltholdighet"; fieldfileUnitMap[307] = "1e-3";
fieldfileParamMap[308] = "sjoetemperatur"; fieldfileUnitMap[308] = "C";
fieldfileParamMap[309] = "x_komp_av_surface_stress_over_dens"; fieldfileUnitMap[309] = "m^2/s^2";
fieldfileParamMap[310] = "y_komp_av_surface_stress_over_dens"; fieldfileUnitMap[310] = "m^2/s^2";

fieldfileParamMap[312] = "Q2_turb_energi"; fieldfileUnitMap[312] = "m^2/s^2";
fieldfileParamMap[313] = "KM_vertikal_eddy_visc"; fieldfileUnitMap[313] = "m^2/s";
fieldfileParamMap[314] = "AM_horisontal_eddy_visc"; fieldfileUnitMap[314] = "m^2/s";
fieldfileParamMap[315] = "KH_Vertical_diffusion_coeff_for_heat_and_salinity"; fieldfileUnitMap[315] = "1";

fieldfileParamMap[331] = "MIDDEL_overflatehevning"; fieldfileUnitMap[331] = "m";
fieldfileParamMap[332] = "MIDDEL_u_stroem_komponent"; fieldfileUnitMap[332] = "m/s";
fieldfileParamMap[333] = "MIDDEL_v_stroem_komponent"; fieldfileUnitMap[333] = "m/s";

fieldfileParamMap[337] = "MIDDEL_saltholdighet"; fieldfileUnitMap[337] = "1e-3";
fieldfileParamMap[338] = "MIDDEL_sjoetemperatur"; fieldfileUnitMap[338] = "C";

fieldfileParamMap[340] = "is_konsentrasjon"; fieldfileUnitMap[340] = "percent";
fieldfileParamMap[341] = "is_tykkelse"; fieldfileUnitMap[341] = "m";
fieldfileParamMap[342] = "u_isdrivthastighet_i_x_retning"; fieldfileUnitMap[342] = "m/s";
fieldfileParamMap[343] = "v_isdrivthastighet_i_y_retning"; fieldfileUnitMap[342] = "m/s";
fieldfileParamMap[344] = "is_konsentrasjon_ettaars_is"; fieldfileUnitMap[344] = "percent";
fieldfileParamMap[345] = "is_konsentrasjon_fleraars_is"; fieldfileUnitMap[345] = "percent";
fieldfileParamMap[346] = "is_konsentrasjon_tynn_is"; fieldfileUnitMap[346] = "percent";

fieldfileParamMap[351] = "bunntopografi"; fieldfileUnitMap[351] = "m";
fieldfileParamMap[352] = "alfaF"; fieldfileUnitMap[352] = "1";
fieldfileParamMap[353] = "alfaU"; fieldfileUnitMap[353] = "1";
fieldfileParamMap[354] = "alfaV"; fieldfileUnitMap[354] = "1";
fieldfileParamMap[355] = "River_map_1"; fieldfileUnitMap[355] = "1";
fieldfileParamMap[356] = "River_map_2"; fieldfileUnitMap[356] = "1";
fieldfileParamMap[357] = "MPP_processor_map"; fieldfileUnitMap[357] = "1";

fieldfileParamMap[359] = "rmap_map_ratio"; fieldfileUnitMap[359] = "1";

///360 - 389 For vannkvalitet
fieldfileParamMap[371] = "nitrogen"; fieldfileUnitMap[371] = "1";
fieldfileParamMap[372] = "phosphate"; fieldfileUnitMap[372] = "1";
fieldfileParamMap[373] = "silicate"; fieldfileUnitMap[373] = "1";
fieldfileParamMap[374] = "detrius"; fieldfileUnitMap[374] = "1";
fieldfileParamMap[375] = "diatome"; fieldfileUnitMap[375] = "1";
fieldfileParamMap[376] = "flagellate"; fieldfileUnitMap[376] = "1";
fieldfileParamMap[377] = "oxygen"; fieldfileUnitMap[377] = "1";
fieldfileParamMap[378] = "silica"; fieldfileUnitMap[378] = "1";
fieldfileParamMap[379] = "sediment2"; fieldfileUnitMap[379] = "1";
fieldfileParamMap[380] = "sediment1"; fieldfileUnitMap[380] = "1";

///                      391 - 399 For tidevann
fieldfileParamMap[391] = "tidevann_amp_vannstand"; fieldfileUnitMap[391] = "m";
fieldfileParamMap[392] = "tidevann_fase_vannstand"; fieldfileUnitMap[392] = "degrees";
fieldfileParamMap[393] = "tidevann_amp_stroem_u_komp"; fieldfileUnitMap[393] = "m/s";
fieldfileParamMap[394] = "tidevann_fase_stroem_u_komp"; fieldfileUnitMap[394] = "degrees";
fieldfileParamMap[395] = "tidevann_amp_stroem_v_komp"; fieldfileUnitMap[395] = "m/s";
fieldfileParamMap[396] = "tidevann_fase_stroem_v_komp"; fieldfileUnitMap[396] = "degrees";

///                  400-499 FOR EMEP MODEL(S)

///                  500-599 FOR SNOE
fieldfileParamMap[500] = "instant_height_of_boundary_layer"; fieldfileUnitMap[500] = "m";
fieldfileParamMap[501] = "average_height_of_boundary_layer"; fieldfileUnitMap[501] = "m";
fieldfileParamMap[502] = "precipitation_accummulated_between_field_output"; fieldfileUnitMap[502] = "mm";
fieldfileParamMap[510] = "instant_concentration_in_boundary_layer"; fieldfileUnitMap[503] = "Bq/m^3";
fieldfileParamMap[511] = "average_concentration_in_boundary_layer"; fieldfileUnitMap[504] = "Bq/m^3";
fieldfileParamMap[512] = "dry_deposition_for_one_time_interval"; fieldfileUnitMap[512] = "Bq/m^2";
fieldfileParamMap[513] = "wet_deposition_for_one_time_interval"; fieldfileUnitMap[513] = "Bq/m^2";
fieldfileParamMap[514] = "dry_deposition_accumulated_from_start"; fieldfileUnitMap[514] = "Bq/m^2";
fieldfileParamMap[515] = "wet_deposition_accumulated_from_start"; fieldfileUnitMap[515] = "Bq/m^2";
fieldfileParamMap[516] = "instant_part_of_mass_in_the_boundary_layer"; fieldfileUnitMap[516] = "percent";
fieldfileParamMap[517] = "average_part_of_mass_in_the_boundary_layer"; fieldfileUnitMap[517] = "percent";
fieldfileParamMap[518] = "accumulated_concentration_in_boundary_layer"; fieldfileUnitMap[518] = "Bq*hour/m^3";
fieldfileParamMap[519] = "instant_concentration_in_boundary_layer"; fieldfileUnitMap[519] = "Bq/m^3";
fieldfileParamMap[521] = "bomb_dry_deposition"; fieldfileUnitMap[521] = "percent";
fieldfileParamMap[522] = "bomb_wet_deposition"; fieldfileUnitMap[522] = "percent";
fieldfileParamMap[523] = "bomb_accumulated_dry_deposition"; fieldfileUnitMap[523] = "percent";
fieldfileParamMap[524] = "bomb_accumulated_wet_deposition"; fieldfileUnitMap[524] = "percent";
///540-569 : instant concentration in each layer (10^-12 g/m3)";
///570-599 : average concentration in each layer (10^-12 g/m3)";


///                  600-699 FOR FELT AVLEDET FRA FJERNMAATE DATA
fieldfileParamMap[641] = "ice_type"; fieldfileUnitMap[641] = "1";
fieldfileParamMap[642] = "ice_concentration"; fieldfileUnitMap[642] = "percent";
fieldfileParamMap[643] = "???"; fieldfileUnitMap[643] = "1";
fieldfileParamMap[644] = "ice_edge_type"; fieldfileUnitMap[644] = "1";
fieldfileParamMap[645] = "???"; fieldfileUnitMap[645] = "1";


///                  700-799 FOR FELT AVLEDET FRA ENSEMBLE PREDICTION SYSTEM
fieldfileParamMap[720] = "Temp_anomaly_less_than_2K_TALM2K"; fieldfileUnitMap[720] = "percent";
fieldfileParamMap[721] = "Temp_anomaly_of_at_least_2K_TALG2K"; fieldfileUnitMap[721] = "percent";
fieldfileParamMap[722] = "Temp_anomaly_less_than_minus_8K_TALM8K"; fieldfileUnitMap[722] = "percent";
fieldfileParamMap[723] = "Temp_anomaly_less_than_minus_4K_TALM4K"; fieldfileUnitMap[723] = "percent";
fieldfileParamMap[724] = "Temp_anomaly_greater_than_4K_TALG4K"; fieldfileUnitMap[724] = "percent";
fieldfileParamMap[725] = "Temp_anomaly_greater_than_8K_TALG8K"; fieldfileUnitMap[725] = "percent";

fieldfileParamMap[760] = "Total_precip_at_least_1mm_TPG1"; fieldfileUnitMap[760] = "percent";
fieldfileParamMap[761] = "Total_precip_at_least_5mm_TPG5"; fieldfileUnitMap[761] = "percent";
fieldfileParamMap[762] = "Total_precip_at_least_10mm_TPG10"; fieldfileUnitMap[762] = "percent";
fieldfileParamMap[763] = "Total_precip_at_least_20mm_TPG20"; fieldfileUnitMap[763] = "percent";
fieldfileParamMap[764] = "Total_precip_less_than_0.1mm_TPL01"; fieldfileUnitMap[764] = "percent";
fieldfileParamMap[765] = "Total_precip_rate_less_than_1mm_over_day_TPRL1"; fieldfileUnitMap[765] = "percent";
fieldfileParamMap[766] = "Total_precip_rate_at_least_3mm_over_day_TPRG3"; fieldfileUnitMap[766] = "percent";
fieldfileParamMap[767] = "Total_precip_rate_at_least_5mm_over_day_TPRG5"; fieldfileUnitMap[767] = "percent";
fieldfileParamMap[768] = "10_metre_speed_at_least_10m_over_s_10SPG10"; fieldfileUnitMap[768] = "percent";
fieldfileParamMap[769] = "10_metre_speed_at_least_15m_over_s_10SPG15"; fieldfileUnitMap[769] = "percent";
fieldfileParamMap[770] = "10_metre_Wind_gust_at_least_15m_over_s_10FGG15"; fieldfileUnitMap[770] = "percent";
fieldfileParamMap[771] = "10_metre_Wind_gust_at_least_20m_over_s_10FGG20"; fieldfileUnitMap[771] = "percent";
fieldfileParamMap[772] = "10_metre_Wind_gust_at_least_25m_over_s_10FGG25"; fieldfileUnitMap[772] = "percent";
fieldfileParamMap[773] = "2_metre_temp_less_than_273.15K_2TL273"; fieldfileUnitMap[773] = "percent";
fieldfileParamMap[774] = "sign_wave_heightat_least_2m_SWHG2"; fieldfileUnitMap[774] = "percent";
fieldfileParamMap[775] = "sign_wave_heightat_least_4m_SWHG4"; fieldfileUnitMap[775] = "percent";
fieldfileParamMap[776] = "sign_wave_heightat_least_6m_SWHG6"; fieldfileUnitMap[776] = "percent";
fieldfileParamMap[777] = "sign_wave_heightat_least_8m_SWHG8"; fieldfileUnitMap[777] = "percent";
fieldfileParamMap[778] = "mean_wave_periodat_least_8s_MWPG8"; fieldfileUnitMap[778] = "percent";
fieldfileParamMap[779] = "mean_wave_periodat_least_10s_MWPG10"; fieldfileUnitMap[779] = "percent";
fieldfileParamMap[780] = "mean_wave_periodat_least_12s_MWPG12"; fieldfileUnitMap[780] = "percent";
fieldfileParamMap[781] = "mean_wave_periodat_least_15s_MWPG15"; fieldfileUnitMap[781] = "percent";

///                  800-899 Monthly and seasonal forecasts
fieldfileParamMap[801] = "2_metre_temp_anomaly"; fieldfileUnitMap[801] = "K";
fieldfileParamMap[802] = "precipitation_Rate_of_acc_anomaly"; fieldfileUnitMap[802] = "mm/week";

fieldfileParamMap[811] = "2_metre_temp"; fieldfileUnitMap[811] = "K";
fieldfileParamMap[812] = "precipitation_Rate_of_accumulation"; fieldfileUnitMap[812] = "mm/week";

///                  900-999 FOR ALLE TYPER MODELLER
fieldfileParamMap[901] = "geografisk_bredde_desimalgrader"; fieldfileUnitMap[901] = "degrees";
fieldfileParamMap[902] = "geografisk_lengde_desimalgrader"; fieldfileUnitMap[902] = "degrees";
fieldfileParamMap[903] = "areal_av_gridrute"; fieldfileUnitMap[903] = "m^2";
fieldfileParamMap[907] = "h�yde_av_spesiell_kjegle_flate"; fieldfileUnitMap[907] = "m";
fieldfileParamMap[908] = "trykk_i_spesiell_kjegle_flate"; fieldfileUnitMap[908] = "hPa";

///		  1000-1999 Diverse atmosfaere
fieldfileParamMap[1035] = "?????_fra_UM"; fieldfileUnitMap[1035] = "1";
fieldfileParamMap[1036] = "?????_fra_UM"; fieldfileUnitMap[1036] = "1";
fieldfileParamMap[1037] = "?????_fra_UM"; fieldfileUnitMap[1037] = "1";
fieldfileParamMap[1040] = "?????_fra_UM"; fieldfileUnitMap[1040] = "1";
fieldfileParamMap[1060] = "?????_fra_UM"; fieldfileUnitMap[1060] = "1";
fieldfileParamMap[1087] = "height_where_max_icing_occurs_pl"; fieldfileUnitMap[1087] = "ft";
fieldfileParamMap[1088] = "bottom_height_where_icing_gt_4_pl"; fieldfileUnitMap[1088] = "ft";
fieldfileParamMap[1089] = "top_height_where_icing_gt_4_pl"; fieldfileUnitMap[1089] = "ft";
fieldfileParamMap[1092] = "icing_index"; fieldfileUnitMap[1092] = "1";
fieldfileParamMap[1093] = "max_icing_index"; fieldfileUnitMap[1093] = "1";
fieldfileParamMap[1094] = "height_where_max_icing_occurs"; fieldfileUnitMap[1094] = "ft";
fieldfileParamMap[1095] = "bottom_height_where_icing_gt_4"; fieldfileUnitMap[1095] = "ft";
fieldfileParamMap[1096] = "top_height_where_icing_gt_4"; fieldfileUnitMap[1096] = "ft";
fieldfileParamMap[1097] = "contrails"; fieldfileUnitMap[1097] = "1";
fieldfileParamMap[1098] = "contrails_sum"; fieldfileUnitMap[1098] = "1";
fieldfileParamMap[1950] = "?????_fra_UM"; fieldfileUnitMap[1950] = "1";
fieldfileParamMap[1951] = "?????_fra_UM"; fieldfileUnitMap[1951] = "1";
fieldfileParamMap[1952] = "?????_fra_UM"; fieldfileUnitMap[1952] = "1";

///2000 + n : n as above, EPS cluster mean or Ensemble mean";
///3000 + n : n as above, EPS cluster std.dev";
///4000 + n : n as above, EPS single runs";

///9000-9999 : PEPS";
}

#endif /*METNOFIELDFILEPARAMMAP_*/
