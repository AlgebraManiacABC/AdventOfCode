import sys
import os
from pathlib import Path
import certifi
from aocd import get_data

os.environ['SSL_CERT_FILE'] = certifi.where()
os.environ['REQUESTS_CA_BUNDLE'] = certifi.where()

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f"Usage:\n\t{sys.argv[0]} <d> <yyyy>")
        sys.exit(-1)

    day = int(sys.argv[1])
    year = int(sys.argv[2])

    # Open folders
    year_folder = Path(f"./{year}")
    if not Path.exists(year_folder):
        Path.mkdir(Path(f"./{year}"),mode=0o775)
    day_folder = Path(f"./{year}/{day}")
    if not Path.exists(day_folder):
        Path.mkdir(Path(f"{Path.cwd()}/{year}/{day}"),mode=0o775)

    Path.write_text(Path(f"{Path.cwd()}/{year}/{day}/input.txt"),
                     get_data(day=day,year=year))
