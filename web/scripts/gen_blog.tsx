import * as MarkdownIt from 'markdown-it'
import * as fs from 'fs'

const openFile = (name: string) => {
  return fs.readFileSync(name, {encoding:'utf8', flag:'r'})
}

const main = () => {
  const dir = '../pages/blog/'
  const gen_dir = dir + '__generated__/'
  fs.rmSync(gen_dir, { recursive: true, force: true })
  fs.mkdirSync(gen_dir)
  const post_dir = dir + 'posts/'
  const md = MarkdownIt.default()

  const files = fs.readdirSync(post_dir)

  for (const file of files) {
    if (file.includes('.md') || file.includes('.markdown')) {
        const markdown = openFile(post_dir + file)
        const html = md.render(markdown)
        fs.writeFileSync(gen_dir + file.replace('.markdown', '.html').replace('.md', '.html'), html, {encoding: 'utf8'})
    }
  }
}

main()

