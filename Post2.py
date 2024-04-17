import requests
import json
import re
import os

# Ignore SSL warnings
requests.packages.urllib3.disable_warnings(requests.packages.urllib3.exceptions.InsecureRequestWarning)

# Authentication information for the source and destination GitLab instances
source_gitlab_url = "https://source-gitlab.com"
source_private_token = "YOUR_SOURCE_PRIVATE_TOKEN"

destination_gitlab_url = "https://destination-gitlab.com"
destination_private_token = "YOUR_DESTINATION_PRIVATE_TOKEN"

# Define the project ID in the destination GitLab instance
project_id = "YOUR_PROJECT_ID"

# Function to create an issue in the destination GitLab instance
def create_issue(issue_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers, json=issue_data, verify=False)
    created_issue = response.json()
    return created_issue

# Function to create a note for an issue in the destination GitLab instance
def create_note(issue_iid, note_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/issues/{issue_iid}/notes", headers=headers, json=note_data, verify=False)
    created_note = response.json()
    return created_note

# Function to download a file from the source GitLab instance
def download_file(file_url):
    headers = {"PRIVATE-TOKEN": source_private_token}
    response = requests.get(f"{source_gitlab_url}/{file_url}", headers=headers, verify=False)
    return response.content

# Function to upload a file to the destination GitLab instance
def upload_file(file_content):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    files = {"file": file_content}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/uploads", headers=headers, files=files, verify=False)
    uploaded_file = response.json()
    return uploaded_file["url"]

# Read issue data from the JSON file
with open("issues_data.json", "r") as json_file:
    all_issues = json.load(json_file)

# Loop through the issues
for issue_data in all_issues:
    # Change the state of the issue to closed if it was closed in the source GitLab
    if issue_data.get("state") == "closed":
        issue_data["state"] = "closed"
    
    # Download and upload files referenced in the issue description
    description = issue_data.get("description", "")
    if description:
        file_urls = re.findall(r"\((uploads/.*?)\)", description)
        for file_url in file_urls:
            file_content = download_file(file_url)
            uploaded_file_url = upload_file(file_content)
            description = description.replace(file_url, uploaded_file_url)
        issue_data["description"] = description
    
    # Create the issue in the destination GitLab
    created_issue = create_issue(issue_data)
    
    # Loop through the notes
    for note_data in issue_data.get("notes", []):
        # Create the note for the issue in the destination GitLab
        create_note(created_issue["iid"], note_data)

print("Issue data has been posted to the new GitLab instance successfully.")
