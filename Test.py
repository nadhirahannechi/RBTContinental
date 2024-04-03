import requests

# Authentication details
old_gitlab_token = 'YOUR_OLD_GITLAB_TOKEN'
new_gitlab_token = 'YOUR_NEW_GITLAB_TOKEN'
old_gitlab_url = 'OLD_GITLAB_URL'
new_gitlab_url = 'NEW_GITLAB_URL'

# Get issues from old GitLab server
headers = {'Private-Token': old_gitlab_token}
old_issues_url = f'{old_gitlab_url}/api/v4/projects/:id/issues'
old_issues_response = requests.get(old_issues_url, headers=headers)
old_issues_data = old_issues_response.json()

# Iterate over old issues and create them on the new GitLab server
for issue in old_issues_data:
    new_issue_data = {
        'title': issue['title'],
        'description': issue['description'],
        # Add other attributes as needed
    }
    new_issues_url = f'{new_gitlab_url}/api/v4/projects/:id/issues'
    headers['Private-Token'] = new_gitlab_token
    new_issue_response = requests.post(new_issues_url, headers=headers, json=new_issue_data)
    if new_issue_response.status_code == 201:
        print(f"Issue '{issue['title']}' migrated successfully.")
    else:
        print(f"Failed to migrate issue '{issue['title']}'.")
