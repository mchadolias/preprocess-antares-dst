import h5py
import uproot
import pandas as pd
from tqdm import tqdm
from datetime import timedelta
import os
import time
import glob

def load_rootfile_to_df(rootfile, columns=None, tree="sel"):
    """
    Load a ROOT file to a pandas DataFrame.

    Args:
        rootfile (str): The path to the ROOT file.
        columns (list, optional): The columns to be loaded. Defaults to None.
        tree (str, optional): The name of the tree in the ROOT file. Defaults to "sel".

    Returns:
        DataFrame: A DataFrame containing the data from the ROOT file.
    """
    print(f"Loading the ROOT file: {rootfile}")
    ctime = time.time()
    
    with uproot.open(rootfile) as f:
        df = f[tree].arrays(columns, library="pd")

    print(f"ROOT file imported as a Dataframe in: {timedelta(seconds=time.time()-ctime)}")
    return df

def load_large_rootfile_to_df(rootfile, columns=None, tree="sel", chunksize=100_000):
    """
    Load a large ROOT file into a pandas DataFrame while optimizing memory usage.

    Parameters:
    file_path (str): The path to the ROOT file.
    tree_name (str): The name of the TTree to load.
    columns (list, optional): A list of columns to load. If None, all columns will be loaded.
    chunksize (int, optional): The number of rows to load in each chunk.

    Returns:
    A pandas DataFrame containing the data from the TTree.
    """
    df_list = []
    
    print(f"Loading the ROOT file: {rootfile}")
    ctime = time.time()
    
    # Open the ROOT file and get the TTree object
    with uproot.open(rootfile) as f:

        # Specify the columns to load
        if columns is None:
            columns = f[tree].keys()

        # Load the data in chunks
        for i in tqdm(range(0, f[tree].num_entries, chunksize)):
            df = f["sel"].arrays(columns, library="pd", entry_start=i, entry_stop=i+chunksize)
            
            # Append the chunk to the list
            df_list.append(df)
    
    print(f"ROOT file imported as a Dataframe in: {timedelta(seconds=time.time()-ctime)}")
    # Concatenate the chunks into a single DataFrame
    return pd.concat(df_list, ignore_index=True)

def print_files(filename, index, num_files):
    """
    Print the file being processed and the percentage of files processed.
    """
    percentage_processed = (index / num_files) * 100
    print(f"Processing file: {filename.ljust(40)} |   Files processed: {percentage_processed:.2f}%")

def load_dataframes(filelist, cuts=None, folder_path = '/sps/km3net/users/jgarcia/NNfit/reconstructions/Taus/'):
    """
    Load DataFrames from files in a folder and apply cuts based on the specified keys and criteria. Inspired by the script provided by Juan Garcia Mendez from the KM3NeT collaboration.

    Parameters:
        list (list): A list containing the HDF5 files (for example '2015').
        cuts (list of dict): A list of dictionaries, where each dictionary contains 'cut_key', 'cut_value', and 'cut_type'.
        folder_path (str): Path to the reconstructions folder. Change it when copy the reconstrcutions to your own storage. It must end with '/'.

    Returns:
        pd.DataFrame: A DataFrame containing the combined data with applied cuts as per the specifications.
    """
    if cuts is None:
        cuts = []

    # Initialize an empty DataFrame
    dfs = []

    # Measure time
    ctime = time.time()
    
    for file in tqdm(filelist):
        if file.endswith(".hdf5"):  # Ensure only data files are processed
            file_path = os.path.join(folder_path, file)
            
            # Load the DataFrame from the file
            df = pd.read_hdf(file_path, index = False)
            
            # Apply cuts as per the specifications
            for cut in cuts:
                cut_key   = cut['cut_key']
                cut_value = cut['cut_value']
                cut_type  = cut['cut_type']
                
                if cut_type == "greater":
                    df = df[df[cut_key] > cut_value]
                elif cut_type == "less":
                    df = df[df[cut_key] < cut_value]
                elif cut_type == "equal":
                    df = df[df[cut_key] == cut_value]
                else:
                    raise ValueError("Invalid cut type")
            
            # Concatenate the DataFrame to the final DataFrame
            dfs.append(df)

    df_final = pd.concat(dfs, ignore_index=True)

    print('Loading time:', timedelta(seconds=time.time()-ctime), '\n')
    return df_final


def list_files_with_pattern(directory, pattern):
    """
    List files in a directory with a specific pattern.

    Args:
        directory (str): directory path to the files
        pattern (str): pattern to match the files

    Returns:
        list: A list of files that match the pattern in the directory.
    """
    files = glob.glob(directory + '/' + pattern)
    print(f"Number of files found: {len(files)}")
    print(files)
    return files

def save_to_hdf5(df, filename, path = '/sps/km3net/users/mchadoli/ANTARES/nnfit_reco/'):
    """
    Export the DataFrame to a HDF5 file.

    Args:
        df (DataFrame): The DataFrame to be exported.
        filename (str): The name of the file to be exported.
        folder_path (str, optional): Directory where the HDF5 file will be exported. 
        Defaults to '/sps/km3net/users/mchadoli/ANTARES/nnfit_reco/'.
    """
    print(f"Exporting the Dataframe to a H5 file as: {filename}")
    ctime = time.time()
    
    path = os.path.join(path, filename)
    df.to_hdf(path, key='df', mode='w')
    
    print(f"DataFrame written to an H5 file as: {filename}")
    print('Exporting time:', timedelta(seconds=time.time()-ctime), '\n')

def rename_h5_df_cols(
    df,
    mapper={
        "RunID": "run_id",
        "EventID": "frame_index",
        "TrigCount": "event_counter_trigger",
    },
):
    return df.rename(columns=mapper)

def export_dataframe_to_rootfile(df, filename, tree = "sel", path = '/home/wecapstor3/capn/mppi133h/ANTARES/mc/cut_selection/low_energy'):
    print(f"\nExporting the DataFrame to a ROOT file as: {filename}")
    ctime = time.time()
    
    with uproot.recreate(os.path.join(path, filename)) as f:
        f[tree] = df.to_records(index = False)
        
    print(f"DataFrame written to a ROOT file as: {filename}")
    print('Exporting time:', timedelta(seconds=time.time()-ctime), '\n')