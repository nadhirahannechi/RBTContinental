import requests

# Authentication information for the GitLab instance
gitlab_url = "https://your-gitlab-instance.com"
private_token = "YOUR_PRIVATE_TOKEN"
project_id = "YOUR_PROJECT_ID"

# Function to delete an issue by its ID
def delete_issue(issue_id):
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.delete(f"{gitlab_url}/api/v4/projects/{project_id}/issues/{issue_id}", headers=headers)
    if response.status_code == 204:
        print(f"Issue {issue_id} deleted successfully.")
    else:
        print(f"Failed to delete issue {issue_id}: {response.text}")

# Function to list all issues in the project and delete them
def delete_all_issues():
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.get(f"{gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers)
    if response.status_code == 200:
        issues = response.json()
        for issue in issues:
            issue_id = issue["id"]
            delete_issue(issue_id)
    else:
        print("Failed to list issues:", response.text)

# Call the function to delete all issues
delete_all_issues()
