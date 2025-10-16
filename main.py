import argparse
def main():
    parser = argparse.ArgumentParser(description= "PyGit - A Simple git clone!")

    subparsers = parser.add_subparsers(dest = "command", help = "Available commands") 

    #init comamnd
    init_parser = subparsers.add_parser("init", help= "Initialize a new repository")

    args = parser.parse_args()
    print(args)


main()