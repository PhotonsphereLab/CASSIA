#include "CASSIA.h"

weather_all readWeatherVariables(const Rcpp::DataFrame& weather, bool spp, bool preles, bool phydro) {
  weather_all weatherVariables;

  /*
   * Read the data
   */

  // Extract variables from the DataFrame
  weatherVariables.TAir = Rcpp::as<std::vector<double>>(weather["T"]);
  weatherVariables.TSoil_A = Rcpp::as<std::vector<double>>(weather["TSA"]);
  weatherVariables.TSoil_B = Rcpp::as<std::vector<double>>(weather["TSB"]);
  weatherVariables.Soil_Moisture = Rcpp::as<std::vector<double>>(weather["MB"]);
  weatherVariables.Precip = Rcpp::as<std::vector<double>>(weather["Rain"]);

  int n = weatherVariables.TAir.size();

  // Initialise all optional vectors to zero so they are safe to index regardless
  // of which photosynthesis mode is active (PAR is read in sugar_model even when
  // photosynthesis_as_input=TRUE and preles=FALSE).
  weatherVariables.Photosynthesis_IN.assign(n, 0.0);
  weatherVariables.PAR.assign(n, 0.0);
  weatherVariables.VPD.assign(n, 0.0);
  weatherVariables.CO2.assign(n, 400.0);
  weatherVariables.fAPAR.assign(n, 0.7);
  weatherVariables.PAR_max.assign(n, 0.0);
  weatherVariables.Nitrogen.assign(n, 0.0);
  weatherVariables.PA.assign(n, 101325.0);
  weatherVariables.SWP.assign(n, 0.0);
  // Daily soil N available for root/mycorrhizal uptake (sugar_model.cpp's
  // nitrogen_uptake() call) - Franklin (2014)'s Nav, g N m-3 d-1
  // (volumetric, a RATE - NOT per kg soil or per m2 ground - see
  // nitrogen_uptake()'s own comment). Default 0.0507 is Hyytiala real
  // data: Korhonen et al. (2013)'s own measured N mass-balance for this
  // exact site gives ~19 kg N ha-1 yr-1 net release of N from
  // decomposition (their Eq. 9/Fig. 4 "Uptake" = 19 from decomposition +
  // 7 from deposition) - converted to g N m-2 yr-1, divided by Franklin's
  // own 150-day growing-season convention (Table S1 footnote: "all rate
  // values refer to the growing season"), then by his effective soil
  // depth (0.25 m): 19 kgN/ha/yr -> 1.9 gN/m2/yr -> 0.0127 gN/m2/d ->
  // 0.0507 gN/m3/d. This replaces the previous 6.4 default, which was a
  // *standing* extractable-N concentration (korhonen_mineral_N.csv) used
  // as a proxy for Nav's actual definition (Table S1: "maximal N uptake
  // per soil volume", a rate, not a pool size) - that mismatch meant 6.4
  // implicitly assumed the whole standing pool regenerated every single
  // day, inflating uptake ~2 orders of magnitude above Korhonen's own
  // measured per-tree range (confirmed by re-running with both numbers,
  // 2026-11-18). See manuscript/nitrogen_uptake Sect. 2/5 for the full
  // derivation, and the full comparison against Korhonen's measurements.
  weatherVariables.AvailableN.assign(n, 0.0507);
  if (weather.containsElementNamed("AvailableN")) {
    weatherVariables.AvailableN = Rcpp::as<std::vector<double>>(weather["AvailableN"]);
  }

  if (spp) {
    weatherVariables.Photosynthesis_IN = Rcpp::as<std::vector<double>>(weather["P"]);
  }

  if (preles) {
    weatherVariables.PAR = Rcpp::as<std::vector<double>>(weather["PAR"]);
    weatherVariables.VPD = Rcpp::as<std::vector<double>>(weather["VPD"]);
    weatherVariables.CO2 = Rcpp::as<std::vector<double>>(weather["CO2"]);
    weatherVariables.fAPAR = Rcpp::as<std::vector<double>>(weather["fAPAR"]);
  }

  if (phydro) {
    weatherVariables.PAR = Rcpp::as<std::vector<double>>(weather["PPFD"]);
    weatherVariables.PAR_max = Rcpp::as<std::vector<double>>(weather["PPFD_max"]);
    weatherVariables.VPD = Rcpp::as<std::vector<double>>(weather["VPD"]);
    weatherVariables.CO2 = Rcpp::as<std::vector<double>>(weather["CO2"]);
    weatherVariables.Nitrogen = Rcpp::as<std::vector<double>>(weather["Nitrogen"]);
    weatherVariables.PA = Rcpp::as<std::vector<double>>(weather["PA"]);
    weatherVariables.SWP = Rcpp::as<std::vector<double>>(weather["SWP"]);
  }

  /*
   * Print the weather data
   */

  // Print the first three values of each vector
  /*
  std::cout << "First three values of weather variables:" << std::endl;

  auto printFirstThree = [](const std::string& name, const std::vector<double>& values) {
    std::cout << name << ": ";
    for (size_t i = 0; i < std::min(values.size(), size_t(3)); ++i) {
      std::cout << values[i] << (i < 2 ? ", " : "\n");
    }
  };

  printFirstThree("TAir", weatherVariables.TAir);
  printFirstThree("TSoil_A", weatherVariables.TSoil_A);
  printFirstThree("TSoil_B", weatherVariables.TSoil_B);
  printFirstThree("Soil_Moisture", weatherVariables.Soil_Moisture);
  printFirstThree("Precip", weatherVariables.Precip);

  if (spp) {
    printFirstThree("Photosynthesis_IN", weatherVariables.Photosynthesis_IN);
  }

  if (preles || phydro) {
    printFirstThree("PAR", weatherVariables.PAR);
    printFirstThree("VPD", weatherVariables.VPD);
    printFirstThree("CO2", weatherVariables.CO2);
    printFirstThree("fAPAR", weatherVariables.fAPAR);
  }

  if (phydro) {
    printFirstThree("PAR_max", weatherVariables.PAR_max);
    printFirstThree("Nitrogen", weatherVariables.Nitrogen);
    printFirstThree("PA", weatherVariables.PA);
    printFirstThree("SWP", weatherVariables.SWP);
  }
   */

  return weatherVariables;
}
