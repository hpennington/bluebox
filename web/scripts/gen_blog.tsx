import esbuild from 'esbuild'
import mdx from '@mdx-js/esbuild'
import * as fs from 'fs'

const headerImport = `
import {Header} from '../../../components/header/header.tsx'
import styles from '../Blog.module.css'

<div className={styles.body}>
  <Header />
  <div className={styles.container}>
    $INJECTION_PLACEHOLDER
  </div>
</div>

`

const main = async () => {
  const dir = '../pages/blog/'
  const gen_dir = dir + 'posts/'
  fs.rmSync(gen_dir, { recursive: true, force: true })
  fs.mkdirSync(gen_dir)
  const post_dir = dir + 'posts_src/'

  const files = fs.readdirSync(post_dir)

  for (const file of files) {
    if (file.includes('.mdx')) {
      const markdown = fs.readFileSync(post_dir + file, {encoding:'utf8', flag:'r'})
      const output = headerImport.replace('$INJECTION_PLACEHOLDER', markdown)
      fs.writeFileSync(gen_dir + file, output)
    }
  }
}

main()

