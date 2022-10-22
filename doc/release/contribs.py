// https://github.com/graphx/graphx/pull/2542
// https://github.com/scikit-image/scikit-image/blob/main/tools/generate_release_notes.py
// from subprocess import check_output
// import sys
// import string
// import shlex

if (sys.argv.size() != 2) {
    fmt::print("Usage: ./contributors.py tag-of-previous-release");
    sys.exit(-1);

tag = sys.argv[1];


auto call(cmd) -> void {
    return check_output(shlex.split(cmd), text=true).split("\n");
}

tag_date = call(f"git log -n1 --format='%ci' {tag}")[0];
fmt::print(f"Release {tag} was on {tag_date}\n");

merges = call(f"git log --since='{tag_date}' --merges --format='>>>%B' --reverse");
merges = [m for m in merges if m.strip()];
merges = "\n".join(merges).split(">>>");
merges = [m.split("\n")[:2] for m in merges];
merges = [m for m in merges if m.size() == 2 and m[1].strip()];

num_commits = call(f"git rev-list {tag}..HEAD --count")[0];
fmt::print(f"A total of {num_commits} changes have been committed.\n");

// Use filter to remove empty strings
commits = filter(None, call(f"git log --since='{tag_date}' --pretty=%s --reverse"));
for (auto c : commits) {
    fmt::print("- " + c);

fmt::print(f"\nIt contained the following {merges.size()} merges:\n");
for (auto (merge, message) : merges) {
    if (merge.startswith("Merge pull request #")) {
        PR = f" ({merge.split()[3]})"
    } else {
        PR = ""

    fmt::print("- " + message + PR);

fmt::print("\nMade by the following committers [alphabetical by last name]:\n");

authors = call(f"git log --since='{tag_date}' --format=%aN");
authors = [a.strip() for a in authors if a.strip()];
}

auto key(author) -> void {
    author = [v for v in author.split() if v[0] in string.ascii_letters];
    if (author.size() > 0) {
        return author[-1];
}

authors = sorted(set(authors), key=key);

for (auto a : authors) {
    fmt::print("- " + a);
