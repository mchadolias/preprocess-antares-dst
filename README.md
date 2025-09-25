# Preprocess ANTARES DST for SWIM Analysis

## 📖 Overview

This repository contains the preprocessing pipeline developed during my Master’s thesis. Its goal is to convert ANTARES Collaboration DST files into ROOT format suitable for further statistical analysis using the **SWIM** framework.

---

## 📂 Repository Structure

The repository is structured as follows:

```markdown
├── README.md <- This overview and project structure documentation.
│
├── LICENSE <- Open-source license governing the project.
│
├── extract_dst <- Step 1: Convert ANTARES DST files into ROOT format (TTree structure).
│
├── corrections <- Step 2: Apply corrections to the dataset based on initial processing outputs.
│
├── oscillation_weights <- Step 3: Compute oscillation-weighted atmospheric probabilities for neutrino events.
│
├── add_NNFit <- Step 4: Merge NNFit H5 outputs and integrate NNFit data into the samples.
│
├── apply_cuts <- Step 5: Apply specific selection cuts for the analysis.
│
├── add_SWIM_Branches <- Step 6: Modify the dataset for compliance with SWIM input requirements (add needed branches).
│
├── merge <- (Optional) Implement merging strategies across pipeline steps.
│
├── external_library <- Shared Python utility modules used throughout the pipeline.
│
└── standard_template.mk <- Makefile template to compile any C++ code used within the pipeline.
```

## ⚙️ Requirements

The pipeline is built using **Python 3.8+** alongside shell and C++ tooling. Here is a list of the required packages:

- `ROOT`
- `h5py`
- `numpy`
- `pandas`
- `scipy`
- `tqdm`

---

## 📜 License

This project is released under the [MIT License](LICENSE).

---

## 📝 Acknowledgements

Special thanks to the ANTARES Collaboration for providing the necessary data and support. The pipeline has been developed using the **SWIM** framework, an open-source framework for the analysis of neutrino oscillations.

