import os
import platform
from subprocess import run, PIPE

# how your executable should be named
project_name: str = "example"

# arguments you wonna use for testing
testing_arguments = "tmp_test_project"


# build code (don't touch it, unless you know what your doing)
windows: bool = platform.system() == "Windows"

if windows:
	run("rmdir /S /Q build", shell = True)
else:
	run("rm -rf build", shell = True)

if not os.path.exists("build"):
	run("mkdir build", shell=True)

def execute_cmd(cmd: str, description: str):
	conan_result = run(
		cmd,
		shell=True,
		stdout=PIPE,
		stderr=PIPE
	)

	if conan_result.returncode == 0:
		print(description + " was successful!!!\n\n\n")
	else:
		print("---DETAIL ZONE---")
		if conan_result.stdout:
			print(conan_result.stdout)
		if conan_result.stderr:
			print(conan_result.stderr)
		print("---END DETAIL ZONE---")
		
		print("\n\n\n" + description + " has failed! (details are above)")
		exit(1)



cmds = [
			(
				"cmake . -B build",
				"CMake pre generating the stuff necessary for building your project"
			),
			(
				"make -C build",
				"Make trying to build your project form the cmake generated stuff"
			)
		]

for cmd in cmds:
	execute_cmd(cmd[0], cmd[1])

if windows:
	if os.path.exists(f".\\build\\{project_name}.exe"):
		run([f".\\build\\{project_name}.exe {testing_arguments}"], shell=True, check = True)
else:
	if os.path.exists(f"./build/{project_name}"):
		run([f"./build/{project_name} {testing_arguments}"], shell=True, check = True)
# end building code