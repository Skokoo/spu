import os
import subprocess

bin_name = "spu"
if os.path.exists("Makefile"):
    for line in open("Makefile"):
        if line.startswith("OUT"):
            bin_name = line.split("=")[1].strip()
            break

if not os.path.exists(bin_name):
    print(f"building {bin_name} first")
    subprocess.run(["make", "clean"])
    subprocess.run(["make"])

termux_path = "/data/data/com.termux/files/usr/bin"
if os.path.isdir(termux_path):
    dest = os.path.join(termux_path, bin_name)
    print(f"installing to {dest}")
    subprocess.run(["cp", bin_name, dest])
    subprocess.run(["chmod", "+x", dest])
    print("done, now you can run spu anywhere")