#!/usr/bin/env python3
"""Grant OS Login on GCE instance to users listed in CSV. Email from SSH key comment."""

import csv
import subprocess
import sys

VM = "ece456"
ZONE = "us-central1-c"
PROJECT = "peak-segment-476400-k5"
CSV_FILE = "logins.csv"


def main() -> None:
    with open(CSV_FILE, newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        next(reader)  # header
        for row in reader:
            if len(row) < 3:
                continue
            sshkey = row[2].strip()
            if not sshkey:
                continue
            # Email is the comment (last token) of the SSH key line
            email = sshkey.split()[-1].rstrip(",")
            if "@" not in email:
                continue
            print(f"Granting OS Login to {email} on {VM}...")
            r = subprocess.run(
                [
                    "gcloud",
                    "compute",
                    "instances",
                    "add-iam-policy-binding",
                    VM,
                    "--zone",
                    ZONE,
                    "--project",
                    PROJECT,
                    "--member",
                    f"user:{email}",
                    "--role",
                    "roles/compute.osLogin",
                    "--quiet",
                ],
                capture_output=True,
                text=True,
            )
            if r.returncode != 0:
                print(r.stderr or r.stdout, file=sys.stderr)


if __name__ == "__main__":
    main()
