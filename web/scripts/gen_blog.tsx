import yaml from 'js-yaml'
import * as fs from 'fs'

type Header = {
  title: string,
  overview: string,
  slug: string;
  date: string,
  tags: Array<string>,
}

const headerImport = `
import Head from 'next/head'
import {Header} from '../../../components/header/header.tsx'
import styles from '../Blog.module.css'

<div className={styles.body}>
  <Head>
    <title>Bluebox blog</title>
    <meta name="description" content="The Bluebox development and annoucement blog!" />
  </Head>
  <Header />
  <div className={styles.container}>
    <div className={styles.main}>
      $INJECTION_PLACEHOLDER
    </div>
  </div>
</div>

`

const blogTemplate = `
import type { NextPage } from 'next'
import Head from 'next/head'
import Image from 'next/image'
import {Header} from '../../components/header/header'
import {PostOverview} from '../../components/posts/post-overview'
import styles from './Blog.module.css'

const Blog: NextPage = () => {
  return (
    <div className={styles.container}>
      <Head>
        <title>Bluebox blog</title>
        <meta name="description" content="The Bluebox development and annoucement blog!" />
      </Head>

      <main className={styles.main}>
        <Header />
        <h1 className={styles.title}>Latest</h1>
        <p className={styles.description}>A blog about the development of a bluetooth guitar pedal. Also covers product & feature announcements.</p>
        $INJECTION_PLACEHOLDER
      </main>

      <footer className={styles.footer}>
      </footer>
    </div>
  )
}

export default Blog
`

const overviewTemplate = (title: string, overview: string, slug: string, date: string, tag0: string, tag1: string, tag2: string) => `       <PostOverview title={'${title}'} overview={'${overview}'} slug={'blog/posts/${slug}'} tag0={'${tag0}'} tag1={'${tag1}'} tag2={'${tag2}'} date={'${date}'} />`

const main = async () => {
  const dir = '../pages/blog/'
  const gen_dir = dir + 'posts/'
  fs.rmSync(gen_dir, { recursive: true, force: true })
  fs.mkdirSync(gen_dir)
  const post_dir = dir + 'posts_src/'

  const files = fs.readdirSync(post_dir)

  const overviews: Array<Header> = []

  const overviewsString: Array<string> = []

  for (const file of files) {
    if (file.includes('.mdx')) {
      const fileContents = fs.readFileSync(post_dir + file, {encoding:'utf8', flag:'r'})
      const frontMatter = yaml.loadAll(fileContents)
      const markdown = fileContents.replace(/---(.|\n)*?---/, '')
      const output = headerImport.replace('$INJECTION_PLACEHOLDER', markdown)
      fs.writeFileSync(gen_dir + file, output)

      const row = frontMatter[0] as Header
      
      overviews.push(row)
      
    }
  }
  
  const options: Intl.DateTimeFormatOptions = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' };

  for (const overview of overviews.sort((a: Header, b: Header) => new Date(b.date).getTime()- (new Date(a.date).getTime()))) {
    overviewsString.push(overviewTemplate(overview['title'], overview['overview'], overview['slug'], new Date(overview['date']).toLocaleDateString('en-us', options), overview['tags'][0], overview['tags'][1], overview['tags'][2]))    
  }

  const overviewString = overviewsString.reduce((prev: string, curr: string) => prev + '\n' + curr, '')
  const blog = blogTemplate.replace('$INJECTION_PLACEHOLDER', overviewString)
  fs.writeFileSync('../pages/blog/index.tsx', blog)

}

main()

