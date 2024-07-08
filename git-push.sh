#!/bin/bash

# Function to exit if any command fails
exit_on_fail() {
    echo "Failed to $1."
    exit 1
}

# Check for any changes (staged or unstaged)
if [[ $(git status --porcelain) ]]; then
    echo "You have changes. Please review them:"
    git status

    # Ask for a commit message
    read -p "Enter your commit message: " commit_message
    if [[ -z "$commit_message" ]]; then
        echo "Commit message cannot be empty."
        exit 1
    fi

    # Add files to staging and commit
    echo "Adding files to staging..."
    git add .
    echo "Committing changes..."
    git commit -m "$commit_message"
    echo "Changes committed with message: $commit_message"
else
    echo "No changes to commit."
fi

# Display all branches
echo "Available branches:"
git branch -a || exit_on_fail "list branches"

# Get the current branch name
current_branch=$(git rev-parse --abbrev-ref HEAD)
if [[ -z "$current_branch" ]]; then
    echo "Error: Cannot determine current branch."
    exit 1
fi
echo "You are currently on branch: $current_branch"

# Confirm with the user
read -p "Confirm you want to push to '$current_branch' (y/n): " confirm_push
if [[ $confirm_push != "y" ]]; then
    read -p "Enter the branch you want to push to: " target_branch
else
    target_branch=$current_branch
fi

# Check if branch change is needed
if [[ "$target_branch" != "$current_branch" ]]; then
    echo "Switching to branch $target_branch..."
    git checkout $target_branch || exit_on_fail "switch to branch $target_branch"
fi

# Pull latest changes from the target branch
echo "Pulling latest changes from $target_branch..."
git pull origin $target_branch || exit_on_fail "pull latest changes from $target_branch"

# Push the changes
echo "Pushing changes to $target_branch..."
git push origin $target_branch || exit_on_fail "push changes to $target_branch"
echo "Changes successfully pushed to $target_branch."